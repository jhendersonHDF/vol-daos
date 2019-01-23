/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Copyright by The HDF Group.                                               *
 * Copyright by the Board of Trustees of the University of Illinois.         *
 * All rights reserved.                                                      *
 *                                                                           *
 * This file is part of HDF5.  The full HDF5 copyright notice, including     *
 * terms governing use, modification, and redistribution, is contained in    *
 * the files COPYING and Copyright.html.  COPYING can be found at the root   *
 * of the source code distribution tree; Copyright.html can be found at the  *
 * root level of an installed copy of the electronic HDF5 document set and   *
 * is linked from the top-level documents page.  It can also be found at     *
 * http://hdfgroup.org/HDF5/doc/Copyright.html.  If you do not have          *
 * access to either file, you may request a copy from help@hdfgroup.org.     *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/*
 * Programmer:  Neil Fortner <nfortne2@hdfgroup.org>
 *              September, 2016
 *
 * Purpose:	The private header file for the DAOS VOL connector.
 */
#ifndef daos_vol_H
#define daos_vol_H

#define H5_HAVE_EFF 1 /* DSINC */

/* Include package's public header */
#include "daos_vol_public.h"

#ifdef H5_HAVE_EFF

#include "daos.h"

#define HDF5_VOL_DAOS_VERSION_1	1	/* Version number of DAOS VOL connector */

#define H5_VOL_DAOS_CLS_VAL (H5VL_class_value_t) H5_VOL_RESERVED + 2 /* Class value of the DAOS VOL connector as defined in H5VLpublic.h DSINC */

#define H5_DAOS_VOL_NAME "daos"

#endif

#ifdef __cplusplus
extern "C" {
#endif

#ifdef H5_HAVE_EFF

/*
 * Macros
 */
/* Constant Keys */
#define H5_DAOS_INT_MD_KEY "/Internal Metadata"
#define H5_DAOS_MAX_OID_KEY "Max OID"
#define H5_DAOS_CPL_KEY "Creation Property List"
#define H5_DAOS_LINK_KEY "Link"
#define H5_DAOS_TYPE_KEY "Datatype"
#define H5_DAOS_SPACE_KEY "Dataspace"
#define H5_DAOS_ATTR_KEY "/Attribute"
#define H5_DAOS_CHUNK_KEY 0u

#ifdef DV_HAVE_MAP
#define H5_DAOS_KTYPE_KEY "Key Datatype"
#define H5_DAOS_VTYPE_KEY "Value Datatype"
#define H5_DAOS_MAP_KEY "MAP_AKEY"
#endif

/* Stack allocation sizes */
#define H5_DAOS_GH_BUF_SIZE 1024
#define H5_DAOS_FOI_BUF_SIZE 1024
#define H5_DAOS_LINK_VAL_BUF_SIZE 256
#define H5_DAOS_GINFO_BUF_SIZE 256
#define H5_DAOS_DINFO_BUF_SIZE 1024
#define H5_DAOS_TINFO_BUF_SIZE 1024
#define H5_DAOS_SEQ_LIST_LEN 128
#define H5_DAOS_ITER_LEN 128
#define H5_DAOS_ITER_SIZE_INIT (4 * 1024)

/* Definitions for building oids */
#define H5_DAOS_IDX_MASK   0x3fffffffffffffffull
#define H5_DAOS_TYPE_MASK  0xc000000000000000ull
#define H5_DAOS_TYPE_GRP   0x0000000000000000ull
#define H5_DAOS_TYPE_DSET  0x4000000000000000ull
#define H5_DAOS_TYPE_DTYPE 0x8000000000000000ull
#define H5_DAOS_TYPE_MAP   0xc000000000000000ull

/* Macros borrowed from H5Fprivate.h */
#define UINT64ENCODE(p, n) {                           \
   uint64_t _n = (n);                                  \
   size_t _i;                                          \
   uint8_t *_p = (uint8_t*)(p);                        \
                                                       \
   for (_i = 0; _i < sizeof(uint64_t); _i++, _n >>= 8) \
      *_p++ = (uint8_t)(_n & 0xff);                    \
   for (/*void*/; _i < 8; _i++)                        \
      *_p++ = 0;                                       \
   (p) = (uint8_t*)(p) + 8;                            \
}

#define UINT64DECODE(p, n) {                 \
   /* WE DON'T CHECK FOR OVERFLOW! */        \
   size_t _i;                                \
                                             \
   n = 0;                                    \
   (p) += 8;                                 \
   for (_i = 0; _i < sizeof(uint64_t); _i++) \
      n = (n << 8) | *(--p);                 \
   (p) += 8;                                 \
}

/* Decode a variable-sized buffer */
/* (Assumes that the high bits of the integer will be zero) */
#define DECODE_VAR(p, n, l) { \
   size_t _i;                 \
                              \
   n = 0;                     \
   (p) += l;                  \
   for (_i = 0; _i < l; _i++) \
      n = (n << 8) | *(--p);  \
   (p) += l;                  \
}

/* Decode a variable-sized buffer into a 64-bit unsigned integer */
/* (Assumes that the high bits of the integer will be zero) */
#define UINT64DECODE_VAR(p, n, l)     DECODE_VAR(p, n, l)

/* Macro borrowed from H5private.h for defining the _ATTR_UNUSED macro */
#ifdef __cplusplus
#   define DV_ATTR_UNUSED       /*void*/
#else /* __cplusplus */
#if defined(H5_HAVE_ATTRIBUTE) && !defined(__SUNPRO_C)
#   define DV_ATTR_UNUSED       __attribute__((unused))
#else
#   define DV_ATTR_UNUSED       /*void*/
#endif
#endif /* __cplusplus */

/* FAPL property to tell the VOL connector to open a saved snapshot when opening a
 * file */
#ifdef DV_HAVE_SNAP_OPEN_ID
#define H5_DAOS_SNAP_OPEN_ID "daos_snap_open"
#endif

/* DSINC - Exclude map functionality for now */
#undef DV_HAVE_MAP

/* DSINC - There are serious problems in HDF5 when trying to call
 * H5Pregister2/H5Punregister on the H5P_FILE_ACCESS class.
 */
#undef DV_HAVE_SNAP_OPEN_ID

/* DSINC - Link and attribute iteration can't be supported until
 * problems with needing to pass the iteration's root object ID
 * can be solved.
 */
#undef DV_HAVE_LINK_ITERATION
#undef DV_HAVE_ATTR_ITERATION

/*
 * Typedefs
 */
/* DAOS-specific file access properties */
typedef struct H5_daos_fapl_t {
    MPI_Comm            comm;           /* communicator                  */
    MPI_Info            info;           /* file information              */
} H5_daos_fapl_t;

/* Common object and attribute information */
typedef struct H5_daos_item_t {
    H5I_type_t type;
    struct H5_daos_file_t *file;
    int rc;
} H5_daos_item_t;

/* Common object information */
typedef struct H5_daos_obj_t {
    H5_daos_item_t item; /* Must be first */
    daos_obj_id_t oid;
    daos_handle_t obj_oh;
} H5_daos_obj_t;

/* The file struct */
typedef struct H5_daos_file_t {
    H5_daos_item_t item; /* Must be first */
    daos_handle_t coh;
    char *file_name;
    uuid_t uuid;
    unsigned flags;
    daos_handle_t glob_md_oh;
    struct H5_daos_group_t *root_grp;
    uint64_t max_oid;
    hbool_t max_oid_dirty;
    hid_t fcpl_id;
    hid_t fapl_id;
    MPI_Comm comm;
    MPI_Info info;
    int my_rank;
    int num_procs;
    hbool_t collective;
    hid_t vol_id;
    void *vol_info;
} H5_daos_file_t;

/* The group struct */
typedef struct H5_daos_group_t {
    H5_daos_obj_t obj; /* Must be first */
    hid_t gcpl_id;
    hid_t gapl_id;
} H5_daos_group_t;

/* The dataset struct */
typedef struct H5_daos_dset_t {
    H5_daos_obj_t obj; /* Must be first */
    hid_t type_id;
    hid_t space_id;
    hid_t dcpl_id;
    hid_t dapl_id;
} H5_daos_dset_t;

/* The datatype struct */
/* Note we could speed things up a bit by caching the serialized datatype.  We
 * may also not need to keep the type_id around.  -NAF */
typedef struct H5_daos_dtype_t {
    H5_daos_obj_t obj; /* Must be first */
    hid_t type_id;
    hid_t tcpl_id;
    hid_t tapl_id;
} H5_daos_dtype_t;

/* The map struct */
typedef struct H5_daos_map_t {
    H5_daos_obj_t obj; /* Must be first */
    hid_t ktype_id;
    hid_t vtype_id;
} H5_daos_map_t;

/* The attribute struct */
typedef struct H5_daos_attr_t {
    H5_daos_item_t item; /* Must be first */
    H5_daos_obj_t *parent;
    char *name;
    hid_t type_id;
    hid_t space_id;
} H5_daos_attr_t;

/* The link value struct */
typedef struct H5_daos_link_val_t {
    H5L_type_t type;
    union {
        daos_obj_id_t hard;
        char *soft;
    } target;
} H5_daos_link_val_t;

/* Enum to indicate if the supplied read buffer can be used as a type conversion
 * or background buffer */
typedef enum {
    H5_DAOS_TCONV_REUSE_NONE,    /* Cannot reuse buffer */
    H5_DAOS_TCONV_REUSE_TCONV,   /* Use buffer as type conversion buffer */
    H5_DAOS_TCONV_REUSE_BKG      /* Use buffer as background buffer */
} H5_daos_tconv_reuse_t;

/* XXX: The following two definitions are only here until they are
 * moved out of their respective H5Xpkg.h header files and into a
 * more public scope. They are still needed for the DAOS VOL to handle
 * these API calls being made.
 */
typedef enum H5VL_file_optional_t {
    H5VL_FILE_CLEAR_ELINK_CACHE,        /* Clear external link cache               */
    H5VL_FILE_GET_FILE_IMAGE,           /* file image                              */
    H5VL_FILE_GET_FREE_SECTIONS,        /* file free selections                    */
    H5VL_FILE_GET_FREE_SPACE,           /* file freespace                          */
    H5VL_FILE_GET_INFO,                 /* file info                               */
    H5VL_FILE_GET_MDC_CONF,             /* file metadata cache configuration       */
    H5VL_FILE_GET_MDC_HR,               /* file metadata cache hit rate            */
    H5VL_FILE_GET_MDC_SIZE,             /* file metadata cache size                */
    H5VL_FILE_GET_SIZE,                 /* file size                               */
    H5VL_FILE_GET_VFD_HANDLE,           /* file VFD handle                         */
    H5VL_FILE_GET_FILE_ID,              /* retrieve or resurrect file ID of object */
    H5VL_FILE_RESET_MDC_HIT_RATE,       /* get metadata cache hit rate             */
    H5VL_FILE_SET_MDC_CONFIG,           /* set metadata cache configuration        */
    H5VL_FILE_GET_METADATA_READ_RETRY_INFO,
    H5VL_FILE_START_SWMR_WRITE,
    H5VL_FILE_START_MDC_LOGGING,
    H5VL_FILE_STOP_MDC_LOGGING,
    H5VL_FILE_GET_MDC_LOGGING_STATUS,
    H5VL_FILE_FORMAT_CONVERT,
    H5VL_FILE_RESET_PAGE_BUFFERING_STATS,
    H5VL_FILE_GET_PAGE_BUFFERING_STATS,
    H5VL_FILE_GET_MDC_IMAGE_INFO,
    H5VL_FILE_GET_EOA,
    H5VL_FILE_INCR_FILESIZE,
    H5VL_FILE_SET_LIBVER_BOUNDS
} H5VL_file_optional_t;

/* types for object optional VOL operations */
typedef enum H5VL_object_optional_t {
    H5VL_OBJECT_GET_COMMENT,            /* get object comment                   */
    H5VL_OBJECT_GET_INFO,               /* get object info                      */
    H5VL_OBJECT_SET_COMMENT             /* set object comment                   */
} H5VL_object_optional_t;

/*
 * Declarations
 */
extern hid_t H5_DAOS_g;

/* Free list definitions */
/* DSINC - currently no external access to free lists
H5FL_DEFINE_EXTERN(H5_daos_file_t);
H5FL_DEFINE_EXTERN(H5_daos_group_t);
H5FL_DEFINE_EXTERN(H5_daos_dset_t);
H5FL_DEFINE_EXTERN(H5_daos_dtype_t);
H5FL_DEFINE_EXTERN(H5_daos_map_t);
H5FL_DEFINE_EXTERN(H5_daos_attr_t);*/

/* DSINC - Until we determine what to do with free lists,
 * these macros should at least keep the allocations working
 * correctly.
 */
#define H5FL_CALLOC(t) DV_calloc(sizeof(t))
#define H5FL_FREE(t, o) DV_free(o)

#ifdef DV_TRACK_MEM_USAGE
/*
 * Counter to keep track of the currently allocated amount of bytes
 */
extern size_t daos_vol_curr_alloc_bytes;
#endif

/* Pool handle for use with all files */
extern daos_handle_t H5_daos_poh_g;

/* Global variables used to open the pool */
extern MPI_Comm pool_comm_g;

/*
 * Prototypes
 */
/* General routines */
herr_t H5_daos_init(hid_t vipl_id);
void H5_daos_oid_generate(daos_obj_id_t *oid, uint64_t addr,
    H5I_type_t obj_type);
void H5_daos_oid_encode(daos_obj_id_t *oid, uint64_t idx, H5I_type_t obj_type);
H5I_type_t H5_daos_addr_to_type(uint64_t addr);
H5I_type_t H5_daos_oid_to_type(daos_obj_id_t oid);
uint64_t H5_daos_oid_to_idx(daos_obj_id_t oid);
void H5_daos_hash128(const char *name, void *hash);
herr_t H5_daos_write_max_oid(H5_daos_file_t *file);

/* File callbacks */
void *H5_daos_file_create(const char *name, unsigned flags, hid_t fcpl_id,
    hid_t fapl_id, hid_t dxpl_id, void **req);
void *H5_daos_file_open(const char *name, unsigned flags, hid_t fapl_id,
    hid_t dxpl_id, void **req);
/* herr_t H5_daos_file_get(void *file, H5VL_file_get_t get_type, hid_t dxpl_id, void **req, va_list arguments); */
herr_t H5_daos_file_specific(void *_item, H5VL_file_specific_t specific_type,
    hid_t dxpl_id, void **req, va_list arguments);
herr_t H5_daos_file_close(void *_file, hid_t dxpl_id, void **req);

/* Link callbacks */
herr_t H5_daos_link_create(H5VL_link_create_type_t create_type, void *_item,
    const H5VL_loc_params_t *loc_params, hid_t lcpl_id, hid_t lapl_id,
    hid_t dxpl_id, void **req);
herr_t H5_daos_link_specific(void *_item, const H5VL_loc_params_t *loc_params,
    H5VL_link_specific_t specific_type, hid_t dxpl_id, void **req,
    va_list arguments);

/* Other link routines */
herr_t H5_daos_link_write(H5_daos_group_t *grp, const char *name,
    size_t name_len, H5_daos_link_val_t *val);
herr_t H5_daos_link_follow(H5_daos_group_t *grp, const char *name,
    size_t name_len, hid_t dxpl_id, void **req, daos_obj_id_t *oid);

/* Group callbacks */
void *H5_daos_group_create(void *_item, const H5VL_loc_params_t *loc_params,
    const char *name, hid_t gcpl_id, hid_t gapl_id, hid_t dxpl_id, void **req);
void *H5_daos_group_open(void *_item, const H5VL_loc_params_t *loc_params,
    const char *name, hid_t gapl_id, hid_t dxpl_id, void **req);
herr_t H5_daos_group_close(void *_grp, hid_t dxpl_id, void **req);

/* Other group routines */
H5_daos_group_t *H5_daos_group_traverse(H5_daos_item_t *item, const char *path,
    hid_t dxpl_id, void **req, const char **obj_name, void **gcpl_buf_out,
    uint64_t *gcpl_len_out);
void *H5_daos_group_create_helper(H5_daos_file_t *file, hid_t gcpl_id,
    hid_t gapl_id, hid_t dxpl_id, void **req, H5_daos_group_t *parent_grp,
    const char *name, size_t name_len, hbool_t collective);
void *H5_daos_group_open_helper(H5_daos_file_t *file, daos_obj_id_t oid,
    hid_t gapl_id, hid_t dxpl_id, void **req, void **gcpl_buf_out,
    uint64_t *gcpl_len_out);
void *H5_daos_group_reconstitute(H5_daos_file_t *file, daos_obj_id_t oid,
    uint8_t *gcpl_buf, hid_t gapl_id, hid_t dxpl_id, void **req);

/* Dataset callbacks */
void *H5_daos_dataset_create(void *_item, const H5VL_loc_params_t *loc_params,
    const char *name, hid_t dcpl_id, hid_t dapl_id, hid_t dxpl_id, void **req);
void *H5_daos_dataset_open(void *_item, const H5VL_loc_params_t *loc_params,
    const char *name, hid_t dapl_id, hid_t dxpl_id, void **req);
herr_t H5_daos_dataset_read(void *_dset, hid_t mem_type_id, hid_t mem_space_id,
    hid_t file_space_id, hid_t dxpl_id, void *buf, void **req);
herr_t H5_daos_dataset_write(void *_dset, hid_t mem_type_id, hid_t mem_space_id,
    hid_t file_space_id, hid_t dxpl_id, const void *buf, void **req);
/* herr_t H5_daos_dataset_specific(void *_dset, H5VL_dataset_specific_t specific_type,
                                        hid_t dxpl_id, void **req, va_list arguments);*/
herr_t H5_daos_dataset_get(void *_dset, H5VL_dataset_get_t get_type,
    hid_t dxpl_id, void **req, va_list arguments);
herr_t H5_daos_dataset_close(void *_dset, hid_t dxpl_id, void **req);

/* Datatype callbacks */
void *H5_daos_datatype_commit(void *obj, const H5VL_loc_params_t *loc_params,
    const char *name, hid_t type_id, hid_t lcpl_id, hid_t tcpl_id,
    hid_t tapl_id, hid_t dxpl_id, void **req);
void *H5_daos_datatype_open(void *_item, const H5VL_loc_params_t *loc_params,
    const char *name, hid_t tapl_id, hid_t dxpl_id, void **req);
herr_t H5_daos_datatype_get(void *obj, H5VL_datatype_get_t get_type,
    hid_t dxpl_id, void **req, va_list arguments);
herr_t H5_daos_datatype_close(void *_dtype, hid_t dxpl_id, void **req);

/* Other datatype routines */
herr_t H5_daos_tconv_init(hid_t src_type_id, size_t *src_type_size,
    hid_t dst_type_id, size_t *dst_type_size, size_t num_elem, void **tconv_buf,
    void **bkg_buf, H5_daos_tconv_reuse_t *reuse, hbool_t *fill_bkg);

/* Object callbacks */
void *H5_daos_object_open(void *_item, const H5VL_loc_params_t *loc_params,
    H5I_type_t *opened_type, hid_t dxpl_id, void **req);
herr_t H5_daos_object_optional(void *_item, hid_t dxpl_id, void **req,
    va_list arguments);

/* Other object routines */
herr_t H5_daos_object_close(void *_obj, hid_t dxpl_id, void **req);

/* Attribute callbacks */
void *H5_daos_attribute_create(void *_obj, const H5VL_loc_params_t *loc_params,
    const char *name, hid_t acpl_id, hid_t aapl_id, hid_t dxpl_id, void **req);
void *H5_daos_attribute_open(void *_obj, const H5VL_loc_params_t *loc_params,
    const char *name, hid_t aapl_id, hid_t dxpl_id, void **req);
herr_t H5_daos_attribute_read(void *_attr, hid_t mem_type_id, void *buf,
    hid_t dxpl_id, void **req);
herr_t H5_daos_attribute_write(void *_attr, hid_t mem_type_id, const void *buf,
    hid_t dxpl_id, void **req);
herr_t H5_daos_attribute_get(void *_item, H5VL_attr_get_t get_type,
    hid_t dxpl_id, void **req, va_list arguments);
herr_t H5_daos_attribute_specific(void *_item,
    const H5VL_loc_params_t *loc_params, H5VL_attr_specific_t specific_type,
    hid_t dxpl_id, void **req, va_list arguments);
herr_t H5_daos_attribute_close(void *_attr, hid_t dxpl_id, void **req);

#ifdef DV_HAVE_MAP
/* Map callbacks */
H5PLUGIN_DLL void * H5_daos_map_create(void *_item, H5VL_loc_params_t *loc_params, const char *name,
                    hid_t ktype_id, hid_t vtype_id, hid_t mcpl_id, hid_t mapl_id,
                    hid_t dxpl_id, void **req);
H5PLUGIN_DLL void * H5_daos_map_open(void *_item, H5VL_loc_params_t *loc_params, const char *name,
                  hid_t mapl_id, hid_t dxpl_id, void **req);
H5PLUGIN_DLL herr_t H5_daos_map_set(void *_map, hid_t key_mem_type_id, const void *key, 
                 hid_t val_mem_type_id, const void *value, hid_t dxpl_id, void **req);
H5PLUGIN_DLL herr_t H5_daos_map_get(void *_map, hid_t key_mem_type_id, const void *key, 
                 hid_t val_mem_type_id, void *value, hid_t dxpl_id, void **req);
H5PLUGIN_DLL herr_t H5_daos_map_get_types(void *_map, hid_t *key_type_id, hid_t *val_type_id, void **req);
H5PLUGIN_DLL herr_t H5_daos_map_get_count(void *_map, hsize_t *count, void **req);
H5PLUGIN_DLL herr_t H5_daos_map_exists(void *_map, hid_t key_mem_type_id, const void *key, 
                    hbool_t *exists, void **req);
H5PLUGIN_DLL herr_t H5_daos_map_close(void *_map, hid_t dxpl_id, void **req);
#endif /* DV_HAVE_MAP */

#endif /* H5_HAVE_EFF */

#ifdef __cplusplus
}
#endif

#endif /* daos_vol_H */
