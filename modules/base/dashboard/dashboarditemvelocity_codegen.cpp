
// This file has been auto-generated by the codegen tool by running codegen either
// directly or indirectly on:  dashboarditemvelocity.cpp
//
// Do not change this file manually as any change will be automatically overwritten,
// instead change the struct tagged with "codegen::Dictionary" in the main file from which
// the code in this file was generated.
//
// If a compiler error brought you here, a struct tagged with "codegen::Dictionary"
// was changed without the codegen tool being run again.

namespace codegen {
template <typename T> openspace::documentation::Documentation doc() {
    static_assert(sizeof(T) == 0); // This should never be called
}
template <> openspace::documentation::Documentation doc<openspace::DashboardItemVelocity>() {
    using namespace openspace::documentation;
    TableVerifier* codegen_Parameters = new TableVerifier;
    codegen_Parameters->documentations.push_back({"Simplification",new BoolVerifier,Optional::Yes,SimplificationInfo.description});
    codegen_Parameters->documentations.push_back({"RequestedUnit",new InListVerifier<StringVerifier>({unitList()}),Optional::Yes,RequestedUnitInfo.description});

    openspace::documentation::Documentation d = {
        "DashboardItemVelocity",
        "DashboardItemVelocity",
        std::move(codegen_Parameters->documentations)
    };
    delete codegen_Parameters;
    return d;
}
} // namespace codegen


namespace codegen {
namespace internal {
template<typename T> void bakeTo(const ghoul::Dictionary&, std::string_view, T*) { static_assert(sizeof(T) == 0); } // This should never be called
void bakeTo(const ghoul::Dictionary& d, std::string_view key, bool* val) { *val = d.value<bool>(key); }

template<typename T> void bakeTo(const ghoul::Dictionary& d, std::string_view key, std::optional<T>* val) {
    if (d.hasKey(key)) {
        T v;
        bakeTo(d, key, &v);
        *val = v;
    }
    else *val = std::nullopt;
}
void bakeTo(const ghoul::Dictionary& d, std::string_view key, std::string* val) { *val = d.value<std::string>(key); }

} // namespace internal

template <typename T> T bake(const ghoul::Dictionary& dict) { static_assert(sizeof(T) == 0); };
template <> Parameters bake<Parameters>(const ghoul::Dictionary& dict) {
    openspace::documentation::testSpecificationAndThrow(codegen::doc<openspace::DashboardItemVelocity>(), dict, "DashboardItemVelocity");
    Parameters res;
    internal::bakeTo(dict, "Simplification", &res.simplification);
    internal::bakeTo(dict, "RequestedUnit", &res.requestedUnit);
    return res;
}
} // namespace codegen
