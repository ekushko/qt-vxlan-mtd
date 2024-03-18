#pragma once

#if defined(VMTD_LIBRARY)
#  define VMTD_SHARED Q_DECL_EXPORT
#else
#  define VMTD_SHARED Q_DECL_IMPORT
#endif
