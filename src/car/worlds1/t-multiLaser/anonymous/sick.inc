
define sicklaser laser
(
  # laser-specific properties
  # factory settings for LMS200	
  range_max 8.0
  fov 90.0
  samples 120

  #samples 90 # still useful but much faster to compute

  # generic model properties
  color "blue"
  size [ 0.15 0.15 0.1 ] # dimensions from LMS200 data sheet	
pose [ 0.225 0.175 0.15 30 ]
)

define sicklaser2 laser
(
  # laser-specific properties
  # factory settings for LMS200	
  range_max 3.0
  fov 45.0
  samples 2

  #samples 90 # still useful but much faster to compute

  # generic model properties
  color "blue"
  size [ 0.4 0.2 0.1 ] # dimensions from LMS200 data sheet
pose [ 0 0 -0.3 -30 ]	
)

define sicklaser3 laser
(
  # laser-specific properties
  # factory settings for LMS200	
  range_max 6.0
  fov 45.0
  samples 1

  #samples 90 # still useful but much faster to compute

  # generic model properties
  color "blue"
  size [ 0.4 0.2 0.1 ] # dimensions from LMS200 data sheet	
 pose [ 0  0 -0.1 30]
)

# extends sicklaser to add nice-looking but relatively expensive geometry
define fancysicklaser sicklaser
(
  # bottom
  block( 
    points 4
    point[0] [ -0.02 -0.077 ]
    point[1] [  0.078 -0.077 ]
    point[2] [  0.078  0.077 ]
    point[3] [ -0.02  0.077 ]
    z [0 0.02 ]
  )

  # back
  block(
    points 4
    point[0] [ -0.078 -0.077 ]
    point[1] [ -0.02  -0.077 ]
    point[2] [ -0.02   0.077 ]
    point[3] [ -0.078  0.077 ]
    z [0 0.21 ]
  )

  # top
  block( points 4
    point[0] [ -0.02 -0.077 ]
    point[1] [  0.078 -0.077 ]
    point[2] [  0.078  0.077 ]
    point[3] [ -0.02  0.077 ]
    z [0.12 0.21 ]  
  )

  # laser bit
  block( points 4
    point[0] [ -0.02 -0.05 ]
	 point[1] [  0.06 -0.05 ]
	 point[2] [  0.06  0.05 ]
	 point[3] [ -0.02  0.05 ]
	 z [0.02 0.12 ]  
   color "gray10"
  )
)



