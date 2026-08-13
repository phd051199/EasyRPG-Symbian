#ifndef EASYRPG_RGSS_CORE_BRIDGE_H
#define EASYRPG_RGSS_CORE_BRIDGE_H

#ifdef __cplusplus
extern "C" {
#endif

enum EasyRpgRgssVersion
{
    ERgssVersionUnknown = 0,
    ERgssVersion1 = 1,
    ERgssVersion2 = 2,
    ERgssVersion3 = 3
};

enum EasyRpgRgssResult
{
    ERgssOk = 0,
    ERgssErrorInvalidArgument = -1,
    ERgssErrorInvalidConfig = -2,
    ERgssErrorNotConfigured = -3,
    ERgssErrorFramebuffer = -4,
    ERgssErrorProjectNotFound = -5,
    ERgssErrorAmbiguousProject = -6,
    ERgssErrorVmUnavailable = -7
};

enum EasyRpgRgssPixelFormat
{
    ERgssPixelRgb565 = 1
};

typedef struct EasyRpgRgssConfig
{
    int version;
    int logical_width;
    int logical_height;
    int output_width;
    int output_height;
    int viewport_x;
    int viewport_y;
    int viewport_width;
    int viewport_height;
    int logic_fps;
    int target_fps;
    int pixel_format;
} EasyRpgRgssConfig;

typedef struct EasyRpgRgssRect
{
    int x;
    int y;
    int width;
    int height;
} EasyRpgRgssRect;

typedef int (*EasyRpgRgssFileExistsFn)(void* context, const char* relative_path);

int EasyRpgRgssDefaultConfig(int version,
                             int output_width,
                             int output_height,
                             EasyRpgRgssConfig* config);
int EasyRpgRgssValidateConfig(const EasyRpgRgssConfig* config);
int EasyRpgRgssDetectProject(EasyRpgRgssFileExistsFn exists_fn,
                             void* context);
void EasyRpgRgssCoreReset(void);
int EasyRpgRgssCoreConfigure(const EasyRpgRgssConfig* config);
int EasyRpgRgssCoreResizeLogical(int logical_width, int logical_height);
int EasyRpgRgssCoreBindFramebuffer(void* pixels,
                                   int width,
                                   int height,
                                   int pitch_bytes,
                                   int pixel_format);
int EasyRpgRgssCoreGetFramebuffer(void** pixels,
                                  int* width,
                                  int* height,
                                  int* pitch_bytes,
                                  int* pixel_format);
int EasyRpgRgssCoreVersion(void);
int EasyRpgRgssCoreGetConfig(EasyRpgRgssConfig* config);
int EasyRpgRgssCoreLogicFps(void);
int EasyRpgRgssCoreTargetFps(void);
int EasyRpgRgssCoreMapPoint(int logical_x,
                            int logical_y,
                            int* output_x,
                            int* output_y);
int EasyRpgRgssCoreMapRect(const EasyRpgRgssRect* logical_rect,
                           EasyRpgRgssRect* output_rect);

#ifdef __cplusplus
}
#endif

#endif
