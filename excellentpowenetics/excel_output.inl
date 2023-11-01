// <copyright file="excel_output.inl" company="Visualisierungsinstitut der Universität Stuttgart">
// Copyright © 2023 Visualisierungsinstitut der Universität Stuttgart. Alle Rechte vorbehalten.
// </copyright>
// <author>Christoph Müller</author>


/*
 * excel_output::invoke
 */
template<class... TArgs>
void excel_output::invoke(_Inout_opt_ VARIANT *outResult,
        _In_ const int automationType,
        _In_ IDispatch *dispatch,
        _In_ LPOLESTR name,
        TArgs&&... args) {
    assert(dispatch != nullptr);

    DISPPARAMS dp = { nullptr, nullptr, 0, 0 }; // Dispatch parameters.
    DISPID dispid_named = DISPID_PROPERTYPUT;   // Input for property puts.
    DISPID dispid = DISPID_UNKNOWN;             // The dispatch ID.

    // Get DISPID for name input.
    {
        auto hr = dispatch->GetIDsOfNames(IID_NULL, &name, 1, 0x040, //LOCALE_USER_DEFAULT,
            &dispid);
        THROW_IF_FAILED(hr);
    }

    // Allocate memory for arguments.
    std::vector<VARIANT> params = { args... };

    // Build DISPPARAMS.
    dp.cArgs = static_cast<UINT>(sizeof...(args));
    dp.rgvarg = params.data();

    // Handle special-case for property puts.
    if ((automationType & DISPATCH_PROPERTYPUT) != 0) {
        dp.cNamedArgs = 1;
        dp.rgdispidNamedArgs = &dispid_named;
    }

    // Make the call.
    {
        auto hr = dispatch->Invoke(dispid, IID_NULL, LOCALE_SYSTEM_DEFAULT,
            automationType, &dp, outResult, nullptr, nullptr);
        THROW_IF_FAILED(hr);
    }
}
