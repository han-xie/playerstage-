MESSAGE( STATUS "Configuring libstageplugin" )

link_directories( ${PLAYER_LIBRARY_DIRS} )
include_directories( ${PLAYER_INCLUDE_DIRS} /usr/include/glib-2.0 /usr/lib/i386-linux-gnu/glib-2.0/include/  /usr/include/libxml2/)

set( stagepluginSrcs 
	     p_driver.h
	     p_driver.cc
	     p_actarray.cc
	     p_blobfinder.cc
	     p_hallsensor.cc
	     p_lightsensor.cc
	     p_graysensor.cc
		 p_gripper.cc
	     p_simulation.cc
	     p_opaque.cc
	     p_laser.cc
	     p_fiducial.cc
	     p_position.cc
	     p_sonar.cc
	     p_speech.cc
	     p_graphics.cc
	     stg_time.cc
	    
	     req_sim.cc  	     
	     confederate.cc  
	     puppet.cc     
	     websim.cc  
	     config.h
	     req_model.cc
	     websim.hh   
	     parser.cc
)

add_library( stagepluginmy MODULE ${stagepluginSrcs} )



target_link_libraries( stagepluginmy
		       stage
		       event
		       xml2
		       glib-2.0
		       ${PLAYER_LIBRARIES}
		       ${OPENGL_LIBRARIES}

)

set_source_files_properties( ${stagepluginSrcs} PROPERTIES COMPILE_FLAGS "${FLTK_CFLAGS}" )

# delete the "lib" prefix from the plugin
SET_TARGET_PROPERTIES( stagepluginmy PROPERTIES PREFIX "" )

IF (BUILD_LSPTEST)
	ADD_SUBDIRECTORY(test)
ENDIF (BUILD_LSPTEST)

INSTALL(TARGETS stagepluginmy
	LIBRARY DESTINATION lib
)

