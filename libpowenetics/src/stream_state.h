// <copyright file="stream_state.h" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2023 Visualisierungsinstitut der Universität Stuttgart. Alle Rechte vorbehalten.
// Licensed under the MIT licence. See LICENCE.txt file in the project root for full licence information.
// </copyright>
// <author>Christoph Müller</author>

#pragma once


/// <summary>
/// Tracks the state of the streaming thread in <see cref="stream_parser_v2" />.
/// </summary>
enum class stream_state {

    /// <summary>
    /// The thread is not running and can only transition to
    /// <see cref="starting" />.
    /// </summary>
    stopped,

    /// <summary>
    /// The thread is starting. Only the streaming thread itself can transition
    /// from here to either <see cref="stopped" /> or <see cref="running" />.
    /// </summary>
    starting,

    /// <summary>
    /// The thread is running. The thread itself can transition to
    /// <see cref="stopped" /> from there in case of an error, or any other
    /// thread can request the thread to stop by transitioning to
    /// <see cref="stopping" />.
    /// </summary>
    running,

    /// <summary>
    /// Requests the thread to stop. This state can only be reached from
    /// <see cref="running" />.
    /// </summary>
    stopping
};
