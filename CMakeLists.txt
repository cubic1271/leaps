cmake_minimum_required(VERSION 3.8)
project(leaps)

set(CMAKE_CXX_STANDARD 11)

set(APP_SOURCE_FILES
        src/leaps/state/title.cpp
        src/leaps/state/init.cpp
        src/leaps/level.cpp
        src/leaps/actor.cpp
        src/leaps/player.cpp
)

set(CORE_SOURCE_FILES
        src/core/leaps.cpp
        src/core/animation.cpp
        src/core/resource.cpp

        src/simple2d/controllers.cpp
        src/simple2d/gl2.cpp
        src/simple2d/gles.cpp
        src/simple2d/input.cpp
        src/simple2d/shapes.cpp
        src/simple2d/sound.cpp
        src/simple2d/text.cpp
        src/simple2d/gl.cpp
        src/simple2d/gl3.cpp
        src/simple2d/image.cpp
        src/simple2d/music.cpp
        src/simple2d/simple2d.cpp
        src/simple2d/sprite.cpp
        src/simple2d/window.cpp
        include/core/util.h 
        src/core/state.cpp 
        src/core/util.cpp 
        src/pugixml/pugixml.cpp 
        src/core/config.cpp 
        src/core/vasprintf.cpp
        
        include/core/twilight.h
        include/core/state.h
        include/core/animation.h
        include/core/resource.h
        include/core/tinydir.h
        include/simple2d/simple2d.h
        include/core/vasprintf.h 
        include/pugixml/pugiconfig.hpp
        )

set(BOX_SOURCE_FILES
        src/Box2D/collision/b2BroadPhase.cpp
        src/Box2D/collision/b2CollideCircle.cpp
        src/Box2D/collision/b2CollideEdge.cpp
        src/Box2D/collision/b2CollidePolygon.cpp
        src/Box2D/collision/b2Collision.cpp
        src/Box2D/collision/b2Distance.cpp
        src/Box2D/collision/b2DynamicTree.cpp
        src/Box2D/collision/b2TimeOfImpact.cpp
        src/Box2D/collision/Shapes/b2ChainShape.cpp
        src/Box2D/collision/Shapes/b2CircleShape.cpp
        src/Box2D/collision/Shapes/b2EdgeShape.cpp
        src/Box2D/collision/Shapes/b2PolygonShape.cpp
        src/Box2D/common/b2BlockAllocator.cpp
        src/Box2D/common/b2Draw.cpp
        src/Box2D/common/b2Math.cpp
        src/Box2D/common/b2Settings.cpp
        src/Box2D/common/b2StackAllocator.cpp
        src/Box2D/common/b2Timer.cpp
        src/Box2D/dynamics/b2Body.cpp
        src/Box2D/dynamics/b2ContactManager.cpp
        src/Box2D/dynamics/b2Fixture.cpp
        src/Box2D/dynamics/b2Island.cpp
        src/Box2D/dynamics/b2World.cpp
        src/Box2D/dynamics/b2WorldCallbacks.cpp
        src/Box2D/dynamics/Contacts/b2ChainAndCircleContact.cpp
        src/Box2D/dynamics/Contacts/b2ChainAndPolygonContact.cpp
        src/Box2D/dynamics/Contacts/b2CircleContact.cpp
        src/Box2D/dynamics/Contacts/b2Contact.cpp
        src/Box2D/dynamics/Contacts/b2ContactSolver.cpp
        src/Box2D/dynamics/Contacts/b2EdgeAndCircleContact.cpp
        src/Box2D/dynamics/Contacts/b2EdgeAndPolygonContact.cpp
        src/Box2D/dynamics/Contacts/b2PolygonAndCircleContact.cpp
        src/Box2D/dynamics/Contacts/b2PolygonContact.cpp
        src/Box2D/dynamics/Joints/b2DistanceJoint.cpp
        src/Box2D/dynamics/Joints/b2FrictionJoint.cpp
        src/Box2D/dynamics/Joints/b2GearJoint.cpp
        src/Box2D/dynamics/Joints/b2Joint.cpp
        src/Box2D/dynamics/Joints/b2MotorJoint.cpp
        src/Box2D/dynamics/Joints/b2MouseJoint.cpp
        src/Box2D/dynamics/Joints/b2PrismaticJoint.cpp
        src/Box2D/dynamics/Joints/b2PulleyJoint.cpp
        src/Box2D/dynamics/Joints/b2RevoluteJoint.cpp
        src/Box2D/dynamics/Joints/b2RopeJoint.cpp
        src/Box2D/dynamics/Joints/b2WeldJoint.cpp
        src/Box2D/dynamics/Joints/b2WheelJoint.cpp
        src/Box2D/rope/b2Rope.cpp
        include/core/particle.h src/core/particle.cpp include/core/config.h)

link_directories(lib/link)
add_definitions(-DGLEW_STATIC)
add_executable(leaps ${APP_SOURCE_FILES} ${CORE_SOURCE_FILES} ${BOX_SOURCE_FILES})

add_custom_command(TARGET leaps POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${PROJECT_SOURCE_DIR}/lib/dist"
        $<TARGET_FILE_DIR:leaps>)

add_custom_command(TARGET leaps POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${PROJECT_SOURCE_DIR}/resource"
        "$<TARGET_FILE_DIR:leaps>/resource")

target_include_directories(leaps PUBLIC lib/include include)
target_link_libraries(leaps SDL2 SDL2_ttf SDL2_mixer SDL2_image glew32 opengl32)
