return {
    -- Pluto barycenter module
    {
        Name = "PlutoBarycenter",
        Parent = "SolarSystemBarycenter",
        -- SphereOfInfluency unit is meters                
		SphereOfInfluency = 1.0E+9,
        Transform = {
            Translation = {
                Type = "SpiceTranslation",
                Body = "PLUTO BARYCENTER",
                Observer = "SUN",
                Kernels = {
                    "${OPENSPACE_DATA}/spice/de430_1850-2150.bsp",
                    "${OPENSPACE_DATA}/spice/plu055.bsp",
                }
            }
        }
    },
    -- Pluto module
    {   
        Name = "Pluto",
        Parent = "PlutoBarycenter",
        -- SphereOfInfluency unit is meters                
		SphereOfInfluency = 1.0E+7,
        Renderable = {
            Type = "RenderablePlanet",
            Frame = "IAU_PLUTO",
            Body = "PLUTO",
            Geometry = {
                Type = "SimpleSphere",
                Radius = 1.173E6,
                Segments = 100
            },
            Textures = {
                Type = "simple",
                Color = "textures/pluto.jpg",
            },
        },
        Transformation = {
            Translation = {
                Type = "SpiceTranslation",
                Body = "PLUTO",
                Observer = "PLUTO BARYCENTER",
                Kernels = "${OPENSPACE_DATA}/spice/plu055.bsp", 
            },
            Rotation = {
                Type = "Spice",
                SourceFrame = "IAU_PLUTO",
                DestinationFrame = "GALACTIC"
            }
        }
    },
    {
        Name = "Charon",
        Parent = "PlutoBarycenter",
        -- SphereOfInfluency unit is meters                
		SphereOfInfluency = 6.0E+6,
        Renderable = {
            Type = "RenderablePlanet",
            Frame = "IAU_CHARON",
            Body = "CHARON",
            Geometry = {
                Type = "SimpleSphere",
                Radius = 6.035E5,
                Segments = 100
            },
            Textures = {
                Type = "simple",
                Color = "textures/gray.jpg",
            },
        },
        Transformation = {
            Translation = {
                Type = "SpiceTranslation",
                Body = "CHARON",
                Observer = "PLUTO BARYCENTER",
                Kernels = "${OPENSPACE_DATA}/spice/plu055.bsp", 
            },
            Rotation = {
                Type = "Spice",
                SourceFrame = "IAU_CHARON",
                DestinationFrame = "GALACTIC"
            }
        }
    },
    -- CharonTrail module
    {   
        Name = "CharonTrail",
        Parent = "PlutoBarycenter",
        Renderable = {
            Type = "RenderableTrailOrbit",
            Translation = {
                Type = "SpiceTranslation",
                Body = "CHARON",
                Observer = "PLUTO BARYCENTER",
            },
            Color = {0.00,0.62,1.00},
            Period = 6.38725,
            Resolution = 1000,
        },
    },
    -- PlutoTrail module
    {   
        Name = "PlutoTrailSolarSystem",
        Parent = "SolarSystemBarycenter",
        Renderable = {
            Type = "RenderableTrailOrbit",
            Translation = {
                Type = "SpiceTranslation",
                Body = "PLUTO BARYCENTER",
                Observer = "SUN",
            },
            Color = {0.58, 0.61, 1.00},
            Period = 247.92 * 365.242,
            Resolution = 1000
        },
        GuiName = "/Solar/PlutoTrail"
    },
    {
        Name = "PlutoTrailPluto",
        Parent = "PlutoBarycenter",
        Renderable = {
            Type = "RenderableTrailOrbit",
            Translation = {
                Type = "SpiceTranslation",
                Body = "PLUTO",
                Observer = "PLUTO BARYCENTER",
            },
            Color = {0.58, 0.61, 1.00},
            Period = 6.38725,
            Resolution = 1000
        },
        GuiName = "/Solar/PlutoTrail"
    }
}