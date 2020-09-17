/*
 * tpl-base.h -
 *
 * Basic definitions.
 *
 *-----------------------------------------------------------------------------
 *
 * This file is part of TPL software released under the MIT "Expat" license.
 *
 * Copyright (c) 2020: Éric Thiébaut <https://github.com/emmt/TPL>
 *
 */

#ifndef _TPL_BASE_H
#define _TPL_BASE_H 1

#ifdef __cplusplus
#  define _TPL_EXTERN_C_BEGIN extern "C" {
#  define _TPL_EXTERN_C_END   }
#  if defined(__GNUC__) || defined(__clang__)
#    define restrict __restrict__
#  elif defined(_MSC_VER)
#    define restrict __restrict
#  else
#    define restrict
#  endif
#else
#  define _TPL_EXTERN_C_BEGIN
#  define _TPL_EXTERN_C_END
#endif

#endif /* _TPL_BASE_H */
