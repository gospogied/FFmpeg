#ifndef AVUTIL_HWCONTEXT_DRM_H
#define AVUTIL_HWCONTEXT_DRM_H

#include "frame.h"

typedef struct AVDRMDeviceContext {
    // File descriptor of DRM device to create frames on.
    // If no device is required, set to -1.
    int fd;
} AVDRMDeviceContext;

typedef struct AVDRMFrameDescriptor {
    // Frame format: DRM_FOURCC_* possibly with DRM_FORMAT_MOD_*.
    uint64_t format;

    // File descriptor for the object containing each plane (may be equal).
    int fd[AV_NUM_DATA_POINTERS];
    // Offset of the plane within the corresponding object.
    ptrdiff_t offset[AV_NUM_DATA_POINTERS];
    // Pitch (linesize) of the plane.
    ptrdiff_t pitch[AV_NUM_DATA_POINTERS];
} AVDRMFrameDescriptor;

typedef struct AVDRMFramesContext {
    // Format of the frames in this frames context.
    uint64_t format;

    // Any other funny creation flags?
} AVDRMFramesContext;

#endif /* AVUTIL_HWCONTEXT_DRM_H */
