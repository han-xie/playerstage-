MESSAGE( STATUS "Configuring libmultiplugin" )

link_directories( ${PLAYER_LIBRARY_DIRS} )
include_directories( ${PLAYER_INCLUDE_DIRS} )

set( multipluginSrcs 
	     p_driver.h
	     p_driver.cc
	     p_actarray.cc
	     p_blobfinder.cc
		 p_gripper.cc
	     p_opaque.cc
	     p_laser.cc
	     p_fiducial.cc
	     p_position.cc
	     p_sonar.cc
	     p_speech.cc
	     p_graphics.cc
	     stg_time.cc
)

add_library( multiplugin MODULE ${multipluginSrcs} )



target_link_libraries( multiplugin
		       stage
		       ${PLAYER_LIBRARIES}
		       ${OPENGL_LIBRARIES}

)

set_source_files_properties( ${multipluginSrcs} PROPERTIES COMPILE_FLAGS "${FLTK_CFLAGS}" )

# delete the "lib" prefix from the plugin
SET_TARGET_PROPERTIES( multiplugin PROPERTIES PREFIX "" )

IF (BUILD_LSPTEST)
	ADD_SUBDIRECTORY(test)
ENDIF (BUILD_LSPTEST)

INSTALL(TARGETS multiplugin
	LIBRARY DESTINATION lib
)

