#include <memory>
#include <string>
#include <vector>

#include "easyrpgsymbian/UpstreamLmuBridge.h"
#include "data.h"
#include "ldb_reader.h"
#include "lmt_reader.h"
#include "lmu_reader.h"

namespace {
std::string SourceEncoding(const char* encoding)
{
    return encoding != 0 ? std::string(encoding) : std::string();
}

bool CopyStringBytes(const std::string& source,
                     unsigned char* destination,
                     unsigned int capacity,
                     unsigned int* length)
{
    if (length == 0)
        return false;

    const unsigned int source_length = static_cast<unsigned int>(source.size());
    *length = source_length;
    if (source_length == 0)
        return true;
    if (destination == 0 || source_length > capacity)
        return false;

    for (unsigned int i = 0; i < source_length; ++i)
        destination[i] = static_cast<unsigned char>(source[i]);
    return true;
}

const RPG::Chipset* FindChipset(int chipset_id)
{
    for (std::vector<RPG::Chipset>::const_iterator it = Data::chipsets.begin();
         it != Data::chipsets.end();
         ++it) {
        if (it->ID == chipset_id)
            return &*it;
    }
    return 0;
}

const RPG::Actor* FindActor(int actor_id)
{
    for (std::vector<RPG::Actor>::const_iterator it = Data::actors.begin();
         it != Data::actors.end();
         ++it) {
        if (it->ID == actor_id)
            return &*it;
    }
    return 0;
}

std::unique_ptr<RPG::Map> LoadMap(const char* filename, const char* encoding)
{
    if (filename == 0)
        return std::unique_ptr<RPG::Map>();
    return LMU_Reader::Load(filename, SourceEncoding(encoding));
}
}

extern "C" int EasyRpgUpstreamLoadLmtSummary(
    const char* filename,
    const char* encoding,
    EasyRpgUpstreamLmtSummary* summary)
{
    if (filename == 0 || summary == 0)
        return -1;

    Data::treemap = RPG::TreeMap();
    if (!LMT_Reader::Load(filename, SourceEncoding(encoding)))
        return -2;

    summary->map_count = static_cast<unsigned int>(Data::treemap.maps.size());
    summary->tree_order_count = static_cast<unsigned int>(Data::treemap.tree_order.size());
    summary->active_node = Data::treemap.active_node;
    summary->start_map_id = Data::treemap.start.party_map_id;
    summary->start_x = Data::treemap.start.party_x;
    summary->start_y = Data::treemap.start.party_y;
    return 0;
}

extern "C" int EasyRpgUpstreamLoadLdbSummary(
    const char* filename,
    const char* encoding,
    int selected_chipset_id,
    EasyRpgUpstreamLdbSummary* summary,
    unsigned char* title_name,
    unsigned int title_name_capacity,
    unsigned int* title_name_length,
    unsigned char* system_name,
    unsigned int system_name_capacity,
    unsigned int* system_name_length,
    unsigned char* chipset_name,
    unsigned int chipset_name_capacity,
    unsigned int* chipset_name_length)
{
    if (filename == 0 || summary == 0 ||
        title_name_length == 0 || system_name_length == 0 || chipset_name_length == 0)
        return -1;

    Data::Clear();
    if (!LDB_Reader::Load(filename, SourceEncoding(encoding)))
        return -2;

    summary->actor_count = static_cast<unsigned int>(Data::actors.size());
    summary->chipset_count = static_cast<unsigned int>(Data::chipsets.size());
    summary->common_event_count = static_cast<unsigned int>(Data::commonevents.size());
    summary->initial_actor_id = 0;
    summary->initial_actor_character_index = 0;
    summary->initial_actor_transparent = 0;
    summary->initial_actor_character_name_length = 0;

    if (!Data::system.party.empty()) {
        summary->initial_actor_id = Data::system.party.front();
        const RPG::Actor* actor = FindActor(summary->initial_actor_id);
        if (actor) {
            summary->initial_actor_character_index = actor->character_index;
            summary->initial_actor_transparent = actor->transparent ? 1 : 0;
            unsigned int actor_name_length = static_cast<unsigned int>(actor->character_name.size());
            if (actor_name_length > sizeof(summary->initial_actor_character_name))
                actor_name_length = sizeof(summary->initial_actor_character_name);
            for (unsigned int i = 0; i < actor_name_length; ++i)
                summary->initial_actor_character_name[i] = static_cast<unsigned char>(actor->character_name[i]);
            summary->initial_actor_character_name_length = actor_name_length;
        }
    }

    const RPG::Chipset* chipset = FindChipset(selected_chipset_id);
    if (selected_chipset_id > 0 && chipset == 0)
        return -3;

    if (!CopyStringBytes(Data::system.title_name,
                         title_name,
                         title_name_capacity,
                         title_name_length) ||
        !CopyStringBytes(Data::system.system_name,
                         system_name,
                         system_name_capacity,
                         system_name_length) ||
        !CopyStringBytes(chipset != 0 ? chipset->chipset_name : std::string(),
                         chipset_name,
                         chipset_name_capacity,
                         chipset_name_length))
        return -4;

    return 0;
}

extern "C" int EasyRpgUpstreamReadLmuSummary(
    const char* filename,
    const char* encoding,
    EasyRpgUpstreamLmuSummary* summary)
{
    if (filename == 0 || summary == 0)
        return -1;

    std::unique_ptr<RPG::Map> map = LoadMap(filename, encoding);
    if (!map)
        return -2;

    unsigned int page_count = 0;
    unsigned int command_count = 0;
    for (std::vector<RPG::Event>::const_iterator event = map->events.begin();
         event != map->events.end();
         ++event) {
        page_count += static_cast<unsigned int>(event->pages.size());
        for (std::vector<RPG::EventPage>::const_iterator page = event->pages.begin();
             page != event->pages.end();
             ++page) {
            command_count += static_cast<unsigned int>(page->event_commands.size());
        }
    }

    summary->width = map->width;
    summary->height = map->height;
    summary->chipset_id = map->chipset_id;
    summary->lower_layer_count = static_cast<unsigned int>(map->lower_layer.size());
    summary->upper_layer_count = static_cast<unsigned int>(map->upper_layer.size());
    summary->event_count = static_cast<unsigned int>(map->events.size());
    summary->event_page_count = page_count;
    summary->event_command_count = command_count;
    summary->save_count = static_cast<unsigned int>(map->save_count < 0 ? 0 : map->save_count);
    return 0;
}

extern "C" int EasyRpgUpstreamReadBootSequence(
    const char* filename,
    const char* encoding,
    EasyRpgUpstreamBootSequence* sequence)
{
    if (filename == 0 || sequence == 0)
        return -1;

    std::unique_ptr<RPG::Map> map = LoadMap(filename, encoding);
    if (!map)
        return -2;

    for (std::vector<RPG::Event>::const_iterator event = map->events.begin();
         event != map->events.end();
         ++event) {
        for (std::vector<RPG::EventPage>::const_iterator page = event->pages.begin();
             page != event->pages.end();
             ++page) {
            if (page->trigger != RPG::EventPage::Trigger_auto_start || page->event_commands.size() != 7)
                continue;

            const RPG::EventCommand& wait0 = page->event_commands[0];
            const RPG::EventCommand& show = page->event_commands[1];
            const RPG::EventCommand& move0 = page->event_commands[2];
            const RPG::EventCommand& wait1 = page->event_commands[3];
            const RPG::EventCommand& move1 = page->event_commands[4];
            const RPG::EventCommand& wait2 = page->event_commands[5];
            const RPG::EventCommand& teleport = page->event_commands[6];

            if (wait0.code != RPG::EventCommand::Code::Wait ||
                show.code != RPG::EventCommand::Code::ShowPicture ||
                move0.code != RPG::EventCommand::Code::MovePicture ||
                wait1.code != RPG::EventCommand::Code::Wait ||
                move1.code != RPG::EventCommand::Code::MovePicture ||
                wait2.code != RPG::EventCommand::Code::Wait ||
                teleport.code != RPG::EventCommand::Code::Teleport ||
                wait0.parameters.size() < 1 ||
                show.parameters.size() < 7 ||
                move0.parameters.size() < 15 ||
                wait1.parameters.size() < 1 ||
                move1.parameters.size() < 15 ||
                wait2.parameters.size() < 1 ||
                teleport.parameters.size() < 3)
                continue;

            sequence->wait_before_picture = wait0.parameters[0];
            sequence->picture_x = show.parameters[2];
            sequence->picture_y = show.parameters[3];
            sequence->picture_magnify = show.parameters[5];
            sequence->picture_opacity = show.parameters[6];
            sequence->first_move_x = move0.parameters[2];
            sequence->first_move_y = move0.parameters[3];
            sequence->first_move_magnify = move0.parameters[5];
            sequence->first_move_opacity = move0.parameters[6];
            sequence->first_move_duration = move0.parameters[14];
            sequence->wait_after_first_move = wait1.parameters[0];
            sequence->second_move_x = move1.parameters[2];
            sequence->second_move_y = move1.parameters[3];
            sequence->second_move_magnify = move1.parameters[5];
            sequence->second_move_opacity = move1.parameters[6];
            sequence->second_move_duration = move1.parameters[14];
            sequence->wait_after_second_move = wait2.parameters[0];
            sequence->target_map_id = teleport.parameters[0];
            sequence->target_x = teleport.parameters[1];
            sequence->target_y = teleport.parameters[2];
            sequence->picture_name_length = static_cast<unsigned int>(show.string.size());
            if (sequence->picture_name_length > sizeof(sequence->picture_name))
                return -3;
            for (unsigned int i = 0; i < sequence->picture_name_length; ++i)
                sequence->picture_name[i] = static_cast<unsigned char>(show.string[i]);
            return 0;
        }
    }

    return -4;
}

extern "C" int EasyRpgUpstreamGetChipsetName(
    int chipset_id,
    unsigned char* name,
    unsigned int capacity,
    unsigned int* length)
{
    if (chipset_id <= 0 || length == 0)
        return -1;
    const RPG::Chipset* chipset = FindChipset(chipset_id);
    if (!chipset)
        return -2;
    return CopyStringBytes(chipset->chipset_name, name, capacity, length) ? 0 : -3;
}

extern "C" int EasyRpgUpstreamReadLmuLayers(
    const char* filename,
    const char* encoding,
    short* lower_layer,
    unsigned int lower_layer_capacity,
    unsigned int* lower_layer_length,
    short* upper_layer,
    unsigned int upper_layer_capacity,
    unsigned int* upper_layer_length)
{
    static_assert(sizeof(short) == sizeof(int16_t), "Symbian LMU bridge requires 16-bit short");

    if (filename == 0 || lower_layer_length == 0 || upper_layer_length == 0)
        return -1;

    std::unique_ptr<RPG::Map> map = LoadMap(filename, encoding);
    if (!map)
        return -2;

    const unsigned int lower_count = static_cast<unsigned int>(map->lower_layer.size());
    const unsigned int upper_count = static_cast<unsigned int>(map->upper_layer.size());
    *lower_layer_length = lower_count;
    *upper_layer_length = upper_count;

    if ((lower_count > 0 && (lower_layer == 0 || lower_count > lower_layer_capacity)) ||
        (upper_count > 0 && (upper_layer == 0 || upper_count > upper_layer_capacity)))
        return -3;

    for (unsigned int i = 0; i < lower_count; ++i)
        lower_layer[i] = static_cast<short>(map->lower_layer[i]);
    for (unsigned int i = 0; i < upper_count; ++i)
        upper_layer[i] = static_cast<short>(map->upper_layer[i]);
    return 0;
}
