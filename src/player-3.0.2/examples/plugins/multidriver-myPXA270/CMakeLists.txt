MESSAGE( STATUS "Configuring multidriver-myPXA270" )

link_directories( ${PLAYER_LIBRARY_DIRS} )
include_directories( ${PLAYER_INCLUDE_DIRS} )

set( stagepluginSrcs 
	     multidriver.h
	     multidriver.cc
	     background.cc
	     background.h
	     stage.hh
	     CYZXInter.h
	     p_position.cc
	     p_laser.cc
	     p_cyzxlaser.cc
	     p_opaque.cc
	     p_blobfinder.cc
	     p_hallsensor.cc
	     p_lightsensor.cc
)

add_library( stageplugincyzxpos2db MODULE ${stagepluginSrcs} )



target_link_libraries( stageplugincyzxpos2db
		       stage
		       ${PLAYER_LIBRARIES}
		       ${OPENGL_LIBRARIES}
)

set_source_files_properties( ${stagepluginSrcs} PROPERTIES COMPILE_FLAGS "${FLTK_CFLAGS}" )

# delete the "lib" prefix from the plugin
SET_TARGET_PROPERTIES( stageplugincyzxpos2db PROPERTIES PREFIX "" )

IF (BUILD_LSPTEST)
	ADD_SUBDIRECTORY(test)
ENDIF (BUILD_LSPTEST)

INSTALL(TARGETS stageplugincyzxpos2db
	LIBRARY DESTINATION lib
)

