﻿// <copyright file="linuxsal.h" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2023 - 2024 Visualisierungsinstitut der Universität Stuttgart.
// Licensed under the MIT licence. See LICENCE file for details.
// </copyright>
// <author>Christoph Müller</author>

#pragma once

#if defined(_WIN32)
#error "This header must not be included on Windows."
#endif /* defined(_WIN32) */


// Empty SAL annotations for rancid platforms.
#define _Analysis_assume_(expr)
#define _In_
#define _Inout_
#define _Inout_opt_
#define _Inout_opt_z_
#define _Inout_updates_(cnt)
#define _In_opt_
#define _In_opt_z_
#define _In_reads_(cnt)
#define _In_reads_bytes_(cnt)
#define _In_reads_or_z_(cnt)
#define _In_z_
#define _Out_
#define _Out_opt_
#define _Out_writes_(cnt)
#define _Out_writes_bytes_(cnt)
#define _Out_writes_opt_(cnt)
#define _Out_writes_opt_z_(cnt)
#define _Ret_
#define _Ret_maybenull_
#define _Ret_maybenull_z_
#define _Ret_null_
#define _Ret_valid_
#define _Ret_z_
#define _When_(expr, annotes)
