#include "RgssCoreBridge.h"

namespace
{
struct TRgssCoreState
{
    EasyRpgRgssConfig config;
    void* pixels;
    int pitchBytes;
    int configured;
};

TRgssCoreState gState;

int LogicalWidthForVersion(int version)
{
    if (version == ERgssVersion1)
        return 640;
    if (version == ERgssVersion2 || version == ERgssVersion3)
        return 544;
    return 0;
}

int LogicalHeightForVersion(int version)
{
    if (version == ERgssVersion1)
        return 480;
    if (version == ERgssVersion2 || version == ERgssVersion3)
        return 416;
    return 0;
}

int LogicFpsForVersion(int version)
{
    if (version == ERgssVersion1)
        return 40;
    if (version == ERgssVersion2 || version == ERgssVersion3)
        return 60;
    return 0;
}

int ClampInt(int value, int minimum, int maximum)
{
    if (value < minimum)
        return minimum;
    if (value > maximum)
        return maximum;
    return value;
}

int ScaleFloor(int value, int sourceSize, int targetSize)
{
    return (value * targetSize) / sourceSize;
}

int ScaleCeil(int value, int sourceSize, int targetSize)
{
    if (value <= 0)
        return 0;
    return 1 + ((value * targetSize - 1) / sourceSize);
}
}

extern "C" int EasyRpgRgssDefaultConfig(int version,
                                         int output_width,
                                         int output_height,
                                         EasyRpgRgssConfig* config)
{
    if (!config || output_width <= 0 || output_height <= 0)
        return ERgssErrorInvalidArgument;

    const int logicalWidth = LogicalWidthForVersion(version);
    const int logicalHeight = LogicalHeightForVersion(version);
    if (logicalWidth <= 0 || logicalHeight <= 0)
        return ERgssErrorInvalidArgument;

    int viewportWidth = output_width;
    int viewportHeight = (logicalHeight * output_width) / logicalWidth;
    if (viewportHeight > output_height)
    {
        viewportHeight = output_height;
        viewportWidth = (logicalWidth * output_height + logicalHeight / 2) / logicalHeight;
    }

    if (viewportWidth <= 0 || viewportHeight <= 0)
        return ERgssErrorInvalidConfig;

    config->version = version;
    config->logical_width = logicalWidth;
    config->logical_height = logicalHeight;
    config->output_width = output_width;
    config->output_height = output_height;
    config->viewport_x = (output_width - viewportWidth) / 2;
    config->viewport_y = (output_height - viewportHeight) / 2;
    config->viewport_width = viewportWidth;
    config->viewport_height = viewportHeight;
    config->logic_fps = LogicFpsForVersion(version);
    config->target_fps = 30;
    config->pixel_format = ERgssPixelRgb565;
    return EasyRpgRgssValidateConfig(config);
}

extern "C" int EasyRpgRgssValidateConfig(const EasyRpgRgssConfig* config)
{
    if (!config)
        return ERgssErrorInvalidArgument;
    if (LogicalWidthForVersion(config->version) <= 0 ||
        LogicalHeightForVersion(config->version) <= 0)
        return ERgssErrorInvalidConfig;
    if (config->logical_width <= 0 || config->logical_height <= 0 ||
        config->output_width <= 0 || config->output_height <= 0 ||
        config->viewport_width <= 0 || config->viewport_height <= 0 ||
        config->logic_fps <= 0 || config->target_fps <= 0)
        return ERgssErrorInvalidConfig;
    if (config->viewport_x < 0 || config->viewport_y < 0 ||
        config->viewport_x + config->viewport_width > config->output_width ||
        config->viewport_y + config->viewport_height > config->output_height)
        return ERgssErrorInvalidConfig;
    if (config->pixel_format != ERgssPixelRgb565)
        return ERgssErrorInvalidConfig;
    return ERgssOk;
}

extern "C" int EasyRpgRgssDetectProject(EasyRpgRgssFileExistsFn exists_fn,
                                         void* context)
{
    if (!exists_fn)
        return ERgssErrorInvalidArgument;

    int version = ERgssVersionUnknown;
    int matches = 0;

    if (exists_fn(context, "Data/Scripts.rxdata"))
    {
        version = ERgssVersion1;
        ++matches;
    }
    if (exists_fn(context, "Data/Scripts.rvdata"))
    {
        version = ERgssVersion2;
        ++matches;
    }
    if (exists_fn(context, "Data/Scripts.rvdata2"))
    {
        version = ERgssVersion3;
        ++matches;
    }

    if (matches == 0)
        return ERgssErrorProjectNotFound;
    if (matches != 1)
        return ERgssErrorAmbiguousProject;
    return version;
}

extern "C" void EasyRpgRgssCoreReset(void)
{
    gState.config.version = ERgssVersionUnknown;
    gState.config.logical_width = 0;
    gState.config.logical_height = 0;
    gState.config.output_width = 0;
    gState.config.output_height = 0;
    gState.config.viewport_x = 0;
    gState.config.viewport_y = 0;
    gState.config.viewport_width = 0;
    gState.config.viewport_height = 0;
    gState.config.logic_fps = 0;
    gState.config.target_fps = 0;
    gState.config.pixel_format = 0;
    gState.pixels = 0;
    gState.pitchBytes = 0;
    gState.configured = 0;
}

extern "C" int EasyRpgRgssCoreConfigure(const EasyRpgRgssConfig* config)
{
    const int result = EasyRpgRgssValidateConfig(config);
    if (result != ERgssOk)
        return result;

    gState.config = *config;
    gState.pixels = 0;
    gState.pitchBytes = 0;
    gState.configured = 1;
    return ERgssOk;
}

extern "C" int EasyRpgRgssCoreBindFramebuffer(void* pixels,
                                               int width,
                                               int height,
                                               int pitch_bytes,
                                               int pixel_format)
{
    if (!gState.configured)
        return ERgssErrorNotConfigured;
    if (!pixels || width != gState.config.output_width ||
        height != gState.config.output_height ||
        pitch_bytes < width * 2 || pixel_format != ERgssPixelRgb565)
        return ERgssErrorFramebuffer;

    gState.pixels = pixels;
    gState.pitchBytes = pitch_bytes;
    return ERgssOk;
}

extern "C" int EasyRpgRgssCoreGetFramebuffer(void** pixels,
                                               int* width,
                                               int* height,
                                               int* pitch_bytes,
                                               int* pixel_format)
{
    if (!gState.configured)
        return ERgssErrorNotConfigured;
    if (!gState.pixels || !pixels || !width || !height || !pitch_bytes || !pixel_format)
        return ERgssErrorFramebuffer;
    *pixels = gState.pixels;
    *width = gState.config.output_width;
    *height = gState.config.output_height;
    *pitch_bytes = gState.pitchBytes;
    *pixel_format = gState.config.pixel_format;
    return ERgssOk;
}

extern "C" int EasyRpgRgssCoreVersion(void)
{
    if (!gState.configured)
        return ERgssVersionUnknown;
    return gState.config.version;
}

extern "C" int EasyRpgRgssCoreGetConfig(EasyRpgRgssConfig* config)
{
    if (!gState.configured)
        return ERgssErrorNotConfigured;
    if (!config)
        return ERgssErrorInvalidArgument;
    *config = gState.config;
    return ERgssOk;
}

extern "C" int EasyRpgRgssCoreLogicFps(void)
{
    if (!gState.configured)
        return 0;
    return gState.config.logic_fps;
}

extern "C" int EasyRpgRgssCoreTargetFps(void)
{
    if (!gState.configured)
        return 0;
    return gState.config.target_fps;
}

extern "C" int EasyRpgRgssCoreMapPoint(int logical_x,
                                        int logical_y,
                                        int* output_x,
                                        int* output_y)
{
    if (!gState.configured)
        return ERgssErrorNotConfigured;
    if (!output_x || !output_y)
        return ERgssErrorInvalidArgument;

    const int x = ClampInt(logical_x, 0, gState.config.logical_width);
    const int y = ClampInt(logical_y, 0, gState.config.logical_height);
    *output_x = gState.config.viewport_x +
        ScaleFloor(x, gState.config.logical_width, gState.config.viewport_width);
    *output_y = gState.config.viewport_y +
        ScaleFloor(y, gState.config.logical_height, gState.config.viewport_height);
    return ERgssOk;
}

extern "C" int EasyRpgRgssCoreMapRect(const EasyRpgRgssRect* logical_rect,
                                       EasyRpgRgssRect* output_rect)
{
    if (!gState.configured)
        return ERgssErrorNotConfigured;
    if (!logical_rect || !output_rect || logical_rect->width < 0 || logical_rect->height < 0)
        return ERgssErrorInvalidArgument;

    const int logicalX0 = ClampInt(logical_rect->x, 0, gState.config.logical_width);
    const int logicalY0 = ClampInt(logical_rect->y, 0, gState.config.logical_height);
    const int logicalX1 = ClampInt(logical_rect->x + logical_rect->width,
                                   0,
                                   gState.config.logical_width);
    const int logicalY1 = ClampInt(logical_rect->y + logical_rect->height,
                                   0,
                                   gState.config.logical_height);

    const int outputX0 = gState.config.viewport_x +
        ScaleFloor(logicalX0, gState.config.logical_width, gState.config.viewport_width);
    const int outputY0 = gState.config.viewport_y +
        ScaleFloor(logicalY0, gState.config.logical_height, gState.config.viewport_height);
    const int outputX1 = gState.config.viewport_x +
        ScaleCeil(logicalX1, gState.config.logical_width, gState.config.viewport_width);
    const int outputY1 = gState.config.viewport_y +
        ScaleCeil(logicalY1, gState.config.logical_height, gState.config.viewport_height);

    output_rect->x = outputX0;
    output_rect->y = outputY0;
    output_rect->width = outputX1 >= outputX0 ? outputX1 - outputX0 : 0;
    output_rect->height = outputY1 >= outputY0 ? outputY1 - outputY0 : 0;
    return ERgssOk;
}
