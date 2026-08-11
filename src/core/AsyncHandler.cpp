#include <map>
#include <memory>
#include <string>

#include "async_handler.h"
#include "filefinder.h"

namespace {
std::map<std::string, FileRequestAsync> gRequests;
int gNextId = 0;
int gImportantPending = 0;

FileRequestAsync* GetRequest(const std::string& aPath) {
    std::map<std::string, FileRequestAsync>::iterator it = gRequests.find(aPath);
    return it == gRequests.end() ? 0 : &it->second;
}

FileRequestBinding CreatePending() {
    return std::make_shared<int>(gNextId++);
}
}

namespace AsyncHandler {
void CreateRequestMapping(const std::string&) {
}

FileRequestAsync* RequestFile(const std::string& aFolderName, const std::string& aFileName) {
    const std::string path = FileFinder::MakePath(aFolderName, aFileName);
    FileRequestAsync* request = GetRequest(path);
    if (request)
        return request;

    gRequests[path] = FileRequestAsync(aFolderName, aFileName);
    return GetRequest(path);
}

FileRequestAsync* RequestFile(const std::string& aFileName) {
    return RequestFile(".", aFileName);
}

bool IsImportantFilePending() {
    return gImportantPending > 0;
}
}

FileRequestAsync::FileRequestAsync()
    : state(State_WaitForStart),
      important(false) {
}

FileRequestAsync::FileRequestAsync(const std::string& aFolderName, const std::string& aFileName)
    : directory(aFolderName),
      file(aFileName),
      path(FileFinder::MakePath(aFolderName, aFileName)),
      state(State_WaitForStart),
      important(false) {
}

bool FileRequestAsync::IsReady() const {
    return state == State_DoneSuccess || state == State_DoneFailure;
}

bool FileRequestAsync::IsImportantFile() const {
    return important;
}

void FileRequestAsync::SetImportantFile(bool aImportant) {
    if (important == aImportant)
        return;
    if (!IsReady())
        gImportantPending += aImportant ? 1 : -1;
    important = aImportant;
}

void FileRequestAsync::Start() {
    if (state == State_Pending)
        return;
    if (IsReady()) {
        DownloadDone(true);
        return;
    }

    state = State_Pending;
    DownloadDone(true);
}

void FileRequestAsync::UpdateProgress() {
}

const std::string& FileRequestAsync::GetPath() const {
    return path;
}

FileRequestBinding FileRequestAsync::Bind(void(*aFunc)(FileRequestResult*)) {
    return Bind(std::function<void(FileRequestResult*)>(aFunc));
}

FileRequestBinding FileRequestAsync::Bind(std::function<void(FileRequestResult*)> aFunc) {
    FileRequestBinding pending = CreatePending();
    listeners.push_back(std::make_pair(FileRequestBindingWeak(pending), aFunc));
    return pending;
}

void FileRequestAsync::CallListeners(bool aSuccess) {
    FileRequestResult result;
    result.directory = directory;
    result.file = file;
    result.success = aSuccess;

    for (std::vector<std::pair<FileRequestBindingWeak, std::function<void(FileRequestResult*)> > >::iterator it = listeners.begin();
         it != listeners.end(); ++it) {
        if (!it->first.expired())
            it->second(&result);
    }
    listeners.clear();
}

void FileRequestAsync::DownloadDone(bool aSuccess) {
    const bool pendingImportant = important && !IsReady();
    if (IsReady())
        aSuccess = state == State_DoneSuccess;
    state = aSuccess ? State_DoneSuccess : State_DoneFailure;
    if (pendingImportant && gImportantPending > 0)
        --gImportantPending;
    CallListeners(aSuccess);
}
