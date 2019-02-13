/* SPDX-License-Identifier: BSD-2-Clause */
/*
 * Copyright (c) 2019,Linaro Limited
 */

/*
 * This file provides extensions to functions defined in <stdlib.h>
 */

#ifndef STRING_EXT_H
#define STRING_EXT_H

#include <stddef.h>

/* Overwrite buffer with a fixed pattern and free it. @ptr may be NULL. */
void free_wipe(void *ptr);

#endif /* STRING_EXT_H */
