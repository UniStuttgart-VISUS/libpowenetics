// <copyright file="linuxhresult.h" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2023 Visualisierungsinstitut der Universität Stuttgart. Alle Rechte vorbehalten.
// Licensed under the MIT licence. See LICENCE.txt file in the project root for full licence information.
// </copyright>
// <author>Christoph Müller</author>

#pragma once

#if defined(_WIN32)
#error "This header must not be included on Windows."
#endif /* defined(_WIN32) */

#include <errno.h>


// Declare a custom error code. Note that facility 4 is FACILITY_ITF on Windows,
// which stands for user-defined codes.
#define _POWENETICS_ERROR_CODE(hr) (hr | ((4 << 16) | 0x80000000))

/// <summary>
/// The type used to report the status of the API.
/// </summary>
/// <remarks>
/// This enumeration only defines the constants that we are
/// likely to use. Note that everyhing is mapped to <c>-errno</c> to
/// the extent possible.
/// </remarks>
typedef enum HRESULT_t {
    // Success codes.
    S_OK = 0,
    S_FALSE = 1,

    // Error codes that can be somehow mapped to errno.
    // -EPERM /* Not root* /,
    ERROR_FILE_NOT_FOUND = -ENOENT,
    // -ESRCH/* No such process			*/
    ERROR_IO_PENDING = -EINTR,
    // -EIO /* I/O error				*/
    // -ENXIO /* No such device or address		*/
    ERROR_FILE_TOO_LARGE = -E2BIG,
    // -ENOEXEC /* Exec format error			*/
    E_HANDLE = -EBADF,
    ERROR_NO_MORE_FILES = -ECHILD,
    WSATRY_AGAIN = -EAGAIN,
    E_OUTOFMEMORY = -ENOMEM,
    E_ACCESSDENIED = -EACCES,
    E_POINTER = -EFAULT,
    // - ENOTBLK /* Block device required		*/
    // -EBUSY /* Mount device busy			*/
    ERROR_FILE_EXISTS = -EEXIST,
    // -EXDEV /* Cross-device link			*/
    // -ENODEV /* No such device			*/
    ERROR_DIRECTORY = -ENOTDIR,
    // -EISDIR /* Is a directory			*/
    E_INVALIDARG = -EINVAL,
    //-ENFILE	23	/* File table overflow			*/
    //-EMFILE	24	/* Too many open files			*/
    //-ENOTTY	25	/* Inappropriate ioctl for device	*/
    //-ETXTBSY	26	/* Text file busy			*/
    //-EFBIG	27	/* File too large			*/
    //-ENOSPC	28	/* No space left on device		*/
    //-ESPIPE	29	/* Illegal seek				*/
    //-EROFS	30	/* Read only file system		*/
    //-EMLINK	31	/* Too many links			*/
    //-EPIPE	32	/* Broken pipe				*/
    //-EDOM	33	/* Math arg out of domain of func	*/
    //-ERANGE	34	/* Math result not representable	*/
    //-ENOMSG	35	/* No message of desired type		*/
    //-EIDRM	36	/* Identifier removed			*/
    //-ECHRNG	37	/* Channel number out of range		*/
    //-EL2NSYNC 38	/* Level 2 not synchronized		*/
    //-EL3HLT	39	/* Level 3 halted			*/
    //-EL3RST	40	/* Level 3 reset			*/
    //-ELNRNG	41	/* Link number out of range		*/
    //-EUNATCH 42	/* Protocol driver not attached		*/
    //-ENOCSI	43	/* No CSI structure available		*/
    //-EL2HLT	44	/* Level 2 halted			*/
    //-EDEADLK	45	/* Deadlock condition.			*/
    //-ENOLCK	46	/* No record locks available.		*/
    //-ECANCELED 47	/* Operation canceled			*/
    //-ENOTSUP	48	/* Operation not supported		*/
    //-EDQUOT	49	/* Disc quota exceeded			*/
    //-EBADE	50	/* invalid exchange			*/
    //-EBADR	51	/* invalid request descriptor		*/
    //-EXFULL	52	/* exchange full			*/
    //-ENOANO	53	/* no anode				*/
    //-EBADRQC	54	/* invalid request code			*/
    //-EBADSLT	55	/* invalid slot				*/
    //-EDEADLOCK 56	/* file locking deadlock error		*/
    //-EBFONT	57	/* bad font file fmt			*/
    //-ENOSTR	60	/* Device not a stream			*/
    //-ENODATA	61	/* no data (for no delay io)		*/
    //-ETIME	62	/* timer expired			*/
    //-ENOSR	63	/* out of streams resources		*/
    //-ENONET	64	/* Machine is not on the network	*/
    //-ENOPKG	65	/* Package not installed		*/
    //-EREMOTE	66	/* The object is remote			*/
    //-ENOLINK	67	/* the link has been severed		*/
    //-EADV	68	/* advertise error			*/
    //-ESRMNT	69	/* srmount error			*/
    //-ECOMM	70	/* Communication error on send		*/
    //-EPROTO	71	/* Protocol error			*/
    //-EMULTIHOP 74	/* multihop attempted			*/
    //-EBADMSG 77	/* trying to read unreadable message	*/
    //-ENAMETOOLONG 78	/* path name is too long		*/
    //-EOVERFLOW 79	/* value too large to be stored in data type */
    //-ENOTUNIQ 80	/* given log. name not unique		*/
    //-EBADFD	81	/* f.d. invalid for this operation	*/
    //-EREMCHG	82	/* Remote address changed		*/
    //-ELIBACC	83	/* Can't access a needed shared lib.	*/
    //-ELIBBAD	84	/* Accessing a corrupted shared lib.	*/
    //-ELIBSCN	85	/* .lib section in a.out corrupted.	*/
    //-ELIBMAX	86	/* Attempting to link in too many libs.	*/
    //-ELIBEXEC 87	/* Attempting to exec a shared library.	*/
    //-EILSEQ	88	/* Illegal byte sequence.		*/
    //-ENOSYS	89	/* Unsupported file system operation	*/
    //-ELOOP	90	/* Symbolic link loop			*/
    //-ERESTART 91	/* Restartable system call		*/
    //-ESTRPIPE 92	/* if pipe/FIFO, don't sleep in stream head */
    //-ENOTEMPTY 93	/* directory not empty			*/
    //-EUSERS	94	/* Too many users (for UFS)		*/
    //-ENOTSOCK	95	/* Socket operation on non-socket */
    //-EDESTADDRREQ	96	/* Destination address required */
    //-EMSGSIZE	97	/* Message too long */
    //-EPROTOTYPE	98	/* Protocol wrong type for socket */
    //-ENOPROTOOPT	99	/* Protocol not available */
    //-EPROTONOSUPPORT	120	/* Protocol not supported */
    //-ESOCKTNOSUPPORT	121	/* Socket type not supported */
    //-EOPNOTSUPP	122	/* Operation not supported on socket */
    //-EPFNOSUPPORT	123	/* Protocol family not supported */
    //-EAFNOSUPPORT	124	/* Address family not supported by */
    //-EADDRINUSE	125	/* Address already in use */
    //-EADDRNOTAVAIL	126	/* Can't assign requested address */
    //-ENETDOWN	127	/* Network is down */
    //-ENETUNREACH	128	/* Network is unreachable */
    //-ENETRESET	129	/* Network dropped connection because */
    //-ECONNABORTED	130	/* Software caused connection abort */
    //-ECONNRESET	131	/* Connection reset by peer */
    //-ENOBUFS		132	/* No buffer space available */
    //-EISCONN		133	/* Socket is already connected */
    //-ENOTCONN	134	/* Socket is not connected */
    //-ESHUTDOWN	143	/* Can't send after socket shutdown */
    //-ETOOMANYREFS	144	/* Too many references: can't splice */
    //-ETIMEDOUT	145	/* Connection timed out */
    //-ECONNREFUSED	146	/* Connection refused */
    //-EHOSTDOWN	147	/* Host is down */
    //-EHOSTUNREACH	148	/* No route to host */
    //-EWOULDBLOCK	EAGAIN
    //-EALREADY	149	/* operation already in progress */
    //-EINPROGRESS	150	/* operation now in progress */
    //-ESTALE		151	/* Stale NFS file handle */

    // Error codes we need, but cannot map.
    E_NOTIMPL = _POWENETICS_ERROR_CODE(1),
    E_NOT_VALID_STATE = _POWENETICS_ERROR_CODE(2),
} HRESULT;

#undef _POWENETICS_ERROR_CODE


/// <summary>
/// Answer whether <paramref name="hr" /> indicates failure.
/// </summary>
#define FAILED(hr) ((int) (hr) < 0)

/// <summary>
/// Answer whether <paramref name="hr" /> indicates success.
/// </summary>
#define SUCCEEDED(hr) ((int) (hr) >= 0)
