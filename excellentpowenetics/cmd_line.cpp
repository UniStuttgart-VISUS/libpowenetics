// <copyright file="cmd_line.h" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2023 Visualisierungsinstitut der Universität Stuttgart. Alle Rechte vorbehalten.
// </copyright>
// <author>Christoph Müller</author>

#include "cmd_line.h"


/*
 * cmd_line::cmd_line
 */
cmd_line::cmd_line(_In_ const int argc, _In_reads_(cnt) const wchar_t **argv) 
        : _excel_visible(true) {
}
