set(ACCELERATION_DIR source/acceleration)

set(RAY_TRACER_FILES 
    ${RAY_TRACER_FILES}
    ${ACCELERATION_DIR}/Aggregate.hpp
    ${ACCELERATION_DIR}/SimpleContainer.hpp
	${ACCELERATION_DIR}/BoundingVolumeHierarchy.hpp
	${ACCELERATION_DIR}/BoundingVolumeHierarchy.cpp
)