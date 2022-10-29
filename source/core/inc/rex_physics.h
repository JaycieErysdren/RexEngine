// ========================================================
//
// FILE:			/source/core/inc/physics.h
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Rex Engine
//
// LICENSE:			ACSL 1.4
//
// DESCRIPTION:		Prototypes for physics module.
//
// LAST EDITED:		October 29th, 2022
//
// ========================================================

// Definitions

// one fixed point unit, don't change
#define PHYS_FRACTIONS_PER_UNIT 512

// short for PHYS_FRACTIONS_PER_UNIT
#define PHYS_F PHYS_FRACTIONS_PER_UNIT

// joint size is scaled (size saving)
#define PHYS_JOINT_SIZE_MULTIPLIER 32

// "infinity"
#define PHYS_INFINITY 2147483647

#define PHYS_JOINT_SIZE(joint) ((joint).radius * PHYS_JOINT_SIZE_MULTIPLIER)

// whether or not use length/distance approximation rather than exact 
// calculation (1 is faster but less accurate), beware of possible lower stability
#ifndef PHYS_APPROXIMATE_LENGTH
	#define PHYS_APPROXIMATE_LENGTH 0
#endif

#if !PHYS_APPROXIMATE_LENGTH
	#define PHYS_DISTANCE PHYS_dist
	#define PHYS_LENGTH PHYS_vec3Len
#else
	#define PHYS_DISTANCE PHYS_distApprox
	#define PHYS_LENGTH PHYS_vec3LenApprox
#endif

#ifndef PHYS_LOG
	#define PHYS_LOG(s) Rex_Log("%s", s)
#endif

// Speed, in rex_ints per ticks, that is considered low (used e.g. for auto deactivation of bodies).
#ifndef PHYS_LOW_SPEED
	#define PHYS_LOW_SPEED 30
#endif

// Tension limit, in rex_ints, after which a non-soft body will be reshaped.
// Smaller number will keep more stable shapes but will cost more performance.
#ifndef PHYS_RESHAPE_TENSION_LIMIT
	#define PHYS_RESHAPE_TENSION_LIMIT 20
#endif

// How many iterations of reshaping will be performed by the step function if
// the body's shape needs to be reshaped. Greater number will keep shapes more
// stable but will cost some performance.
#ifndef PHYS_RESHAPE_ITERATIONS
	#define PHYS_RESHAPE_ITERATIONS 3
#endif

// After how many ticks of low speed should a body be disabled. This mustn't be greater than 255.
#ifndef PHYS_DEACTIVATE_AFTER
	#define PHYS_DEACTIVATE_AFTER 128
#endif

// When a body is activated by a collision, its deactivation counter will be
// set to this value, i.e. after a collision the body will be prone to deactivate
// sooner than normally. This is to handle situations with many bodies touching
// each other that would normally keep activating each other, never coming to rest.
#ifndef PHYS_LIGHT_DEACTIVATION
	#define PHYS_LIGHT_DEACTIVATION (PHYS_DEACTIVATE_AFTER - PHYS_DEACTIVATE_AFTER / 10)
#endif

// Number by which the base acceleration (PHYS_FRACTIONS_PER_UNIT per tick
// squared) caused by the connection tension will be divided. This should be power of 2.
#ifndef PHYS_TENSION_ACCELERATION_DIVIDER
	#define PHYS_TENSION_ACCELERATION_DIVIDER 32
#endif

// Limit within which acceleration caused by connection tension won't be applied.
#ifndef PHYS_TENSION_ACCELERATION_THRESHOLD
	#define PHYS_TENSION_ACCELERATION_THRESHOLD 5
#endif

// Connection tension threshold after which twice as much acceleration will
// be applied. This helps prevent diverting joints that are "impaled" by environment.
#ifndef PHYS_TENSION_GREATER_ACCELERATION_THRESHOLD
	#define PHYS_TENSION_GREATER_ACCELERATION_THRESHOLD (PHYS_TENSION_ACCELERATION_THRESHOLD * 3)
#endif

// Maximum number of iterations to try to uncollide two colliding bodies.
#ifndef PHYS_COLLISION_RESOLUTION_ITERATIONS
	#define PHYS_COLLISION_RESOLUTION_ITERATIONS 16
#endif

// Margin, in rex_ints, by which a body will be shifted back to get out of collision.
#ifndef PHYS_COLLISION_RESOLUTION_MARGIN
	#define PHYS_COLLISION_RESOLUTION_MARGIN (PHYS_F / 64)
#endif

// Number of times a collision of nonrotating bodies with environment will be
// attempted to resolve. This probably won't have great performance implications
// as complex collisions of this kind should be relatively rare.
#ifndef PHYS_NONROTATING_COLLISION_RESOLVE_ATTEMPTS
	#define PHYS_NONROTATING_COLLISION_RESOLVE_ATTEMPTS 8
#endif

// Whether to use a fast approximation for calculating net speed of bodies which increases performance a bit.
#ifndef PHYS_APPROXIMATE_NET_SPEED
	#define PHYS_APPROXIMATE_NET_SPEED 1
#endif

// Debug rendering colors
#define PHYS_DEBUG_COLOR_CONNECTION 0
#define PHYS_DEBUG_COLOR_JOINT 1
#define PHYS_DEBUG_COLOR_ENVIRONMENT 2
#define PHYS_DEBUG_COLOR_INACTIVE 3

//
// Function types
//

// Function used for defining static environment, working similarly to an SDF
// (signed distance function). The parameters are: 3D point P, max distance D.
// The function should behave like this: if P is inside the solid environment
// volume, P will be returned; otherwise closest point (by Euclidean distance) to
// the solid environment volume from P will be returned, except for a case when
// this closest point would be further away than D, in which case any arbitrary
// point further away than D may be returned (this allows for optimizations).
typedef rex_vec3i (*PHYS_closest_point_function)(rex_vec3i, rex_int);

// Function that can be used as a joint-joint or joint-environment collision
// callback, parameters are following: body1 index, joint1 index, body2 index,
// joint2 index, collision world position. If body1 index is the same as body1
// index, then collision type is body-environment, otherwise it is body-body
// type. The function has to return either 1 if the collision is to be allowed
// or 0 if it is to be discarded. This can besides others be used to disable
// collisions between some bodies.
typedef rex_ubyte (*PHYS_collision_callback)(rex_ushort, rex_ushort, rex_ushort, rex_ushort, rex_vec3i);

// Function used by the debug drawing functions to draw individual pixels to
// the screen. The parameters are following: pixel x, pixel y, pixel color.
typedef void (*PHYS_debug_draw_function)(rex_ushort, rex_ushort, rex_ubyte);

// Print a rex_vec3i
#define PRINT_VEC3I(v) Rex_Log("[%d %d %d]",(v).x,(v).y,(v).z);

//
// Base types
//

typedef struct
{
	rex_vec3i position;
	rex_short velocity[3];
	rex_ubyte radius;
} PHYS_joint;

typedef struct
{
	rex_ubyte joint1;
	rex_ubyte joint2;
	rex_ushort length;
} PHYS_connection;

typedef struct
{
	PHYS_joint *joints;
	rex_ubyte num_joints;
	PHYS_connection *connections;
	rex_ubyte num_connections;
	rex_short joint_mass;
	rex_short friction;
	rex_short elasticity;
	rex_ubyte flags;
	rex_ubyte deactivate_count;
} PHYS_body;

typedef struct
{
	PHYS_body *bodies;
	rex_ushort num_bodies;
	PHYS_closest_point_function environment_function;
	PHYS_collision_callback collision_callback;
} PHYS_world;

//
// Flags
//

// Not being updated due to low energy, "sleeping", will be woken by collisions etc.
#define PHYS_BODY_FLAG_DEACTIVATED 1

// When set, the body won't rotate, will only move linearly. Here the velocity of the body's first joint is the velocity of the whole body.
#define PHYS_BODY_FLAG_NONROTATING 2

// Disabled, not taking part in simulation.
#define PHYS_BODY_FLAG_DISABLED 4

// Soft connections, effort won't be made to keep the body's shape.
#define PHYS_BODY_FLAG_SOFT 8

// Simple connections, don't zero out antagonist forces or apply connection friction, can increase performance.
#define PHYS_BODY_FLAG_SIMPLE_CONN 16

// Will never deactivate due to low energy.
#define PHYS_BODY_FLAG_ALWAYS_ACTIVE 32

//
// Functions
//

// Keeps given point within specified axis-aligned box. This can be used e.g. to smooth rendered movement of jittering physics bodies.
rex_vec3i PHYS_vec3KeepWithinBox(rex_vec3i point, rex_vec3i boxCenter, rex_vec3i boxMaxVect);
rex_vec3i PHYS_vec3KeepWithinDistanceBand(rex_vec3i point, rex_vec3i center, rex_int minDistance, rex_int maxDistance);

rex_vec3i PHYS_vec3(rex_int x, rex_int y, rex_int z);
rex_vec3i PHYS_vec3Minus(rex_vec3i v1, rex_vec3i v2);
rex_vec3i PHYS_vec3Plus(rex_vec3i v1, rex_vec3i v2);
rex_vec3i PHYS_vec3Cross(rex_vec3i v1, rex_vec3i v2);
rex_vec3i PHYS_vec3Project(rex_vec3i v, rex_vec3i base);
rex_vec3i PHYS_vec3ProjectNormalized(rex_vec3i v, rex_vec3i baseNormalized);
rex_vec3i PHYS_vec3Times(rex_vec3i v, rex_int units);
rex_vec3i PHYS_vec3TimesPlain(rex_vec3i v, rex_int q);
rex_vec3i PHYS_vec3Normalized(rex_vec3i v);

rex_int PHYS_vec3Dot(rex_vec3i v1, rex_vec3i v2);
rex_int PHYS_vec3Len(rex_vec3i v);
rex_int PHYS_vec3LenApprox(rex_vec3i v);

// Returns an angle in rex_ints (see angle conventions) of a 2D vector with the X axis, CCW.
rex_int PHYS_vec2Angle(rex_int x, rex_int y);

// Keeps given value within specified range. This can be used e.g. for movement smoothing.
rex_int PHYS_keepInRange(rex_int x, rex_int xMin, rex_int xMax);

static inline rex_int PHYS_abs(rex_int x);
static inline rex_int PHYS_max(rex_int a, rex_int b);
static inline rex_int PHYS_min(rex_int a, rex_int b);
static inline rex_int PHYS_nonZero(rex_int x);
static inline rex_int PHYS_dist(rex_vec3i p1, rex_vec3i p2);
static inline rex_int PHYS_distApprox(rex_vec3i p1, rex_vec3i p2);
rex_int PHYS_sqrt(rex_int x);

// Compute sine, PHYS_FRACTIONS_PER_UNIT as argument corresponds to 2 * PI radians. Returns a number from -PHYS_FRACTIONS_PER_UNIT to PHYS_FRACTIONS_PER_UNIT.
rex_int PHYS_sin(rex_int x);
rex_int PHYS_cos(rex_int x);
rex_int PHYS_atan(rex_int x);

// Physics body made of spheres (each of same weight but possibly different radia) connected by elastic springs.

// Tests the mathematical validity of given closest point function (function
// representing the physics environment), i.e. whether for example approaching
// some closest point in a straight line keeps approximately the same closest
// point. Note that this function may take a long time to complete, especially
// with higher gridResolution values and more complex environment functions. You
// should use this function to test your environment function, especially if you
// create functions for your own shapes etc. The cornerFrom and cornerTo points
// are corners of an axis-aligned box within which testing will take place,
// gridResolution defines numbers of points (i.e. step length) along each
// dimension to test (recommended e.g. 64), allowedError says error within which
// points will be considered the same (recommended range approx. 10 to 200). If
// testing is successful, 1 is returned, otherwise 0 is returned and the point
// around which error was detected is returned in errorPoint (unless the pointer
// is 0 in which case it is ignored).
rex_ubyte PHYS_testClosestPointFunction(PHYS_closest_point_function f, rex_vec3i cornerFrom, rex_vec3i cornerTo, rex_ubyte gridResolution, rex_short allowedError, rex_vec3i *errorPoint);
void PHYS_bodyInit(PHYS_body *body, PHYS_joint *joints, rex_ubyte num_joints, PHYS_connection *connections, rex_ubyte num_connections, rex_int mass);
void PHYS_worldInit(PHYS_world *world, PHYS_body *bodies, rex_ushort num_bodies, PHYS_closest_point_function environment_function);

// Gets orientation (rotation) of a body from a position of three of its
// joints. The vector from joint1 to joint2 is considered the body's forward
// direction, the vector from joint1 to joint3 its right direction. The returned
// rotation is in Euler angles (see rotation conventions).
rex_vec3i PHYS_bodyGetRotation(const PHYS_body *body, rex_ushort joint1, rex_ushort joint2, rex_ushort joint3);
void PHYS_vec3Normalize(rex_vec3i *v);

// Rotates a 3D point by given Euler angle rotation (see rotation conventions).
rex_vec3i PHYS_pointRotate(rex_vec3i point, rex_vec3i rotation);

// Returns an inverse rotation to given rotation, in Euler angles (see rotation conventions).
rex_vec3i PHYS_rotationInverse(rex_vec3i rotation);

//Returns a connection tension, i.e. a signed percentage difference against desired length (PHYS_FRACTIONS_PER_UNIT means 100%).
static inline rex_int PHYS_connectionTension(rex_int length, rex_int desiredLength);

// Rotates a rotation specified in Euler angles by given axis + angle (see rotation conventions). Returns a rotation in Eurler angles.
rex_vec3i PHYS_rotationRotateByAxis(rex_vec3i rotation, rex_vec3i rotationByAxis);

// Computes the formula of a 1D collision of rigid bodies.
void PHYS_getVelocitiesAfterCollision(rex_int *v1, rex_int *v2, rex_int m1, rex_int m2, rex_int elasticity);

// Computes orientation/rotation (see docs for orientation format) from two
// vectors (which should be at least close to being perpendicular and do NOT
// need to be normalized). This can be used to determine orientation of a body
// from a relative position of its joints.
rex_vec3i PHYS_rotationFromVecs(rex_vec3i forward, rex_vec3i right);
PHYS_joint PHYS_joint_calculate(rex_vec3i position, rex_int size);

// Mostly for internal use, resolves a potential collision of two joints in a
// way that keeps the joints outside provided environment (if the function
// pointer is not 0). Returns 1 if joints collided or 0 otherwise.
rex_ubyte PHYS_jointsResolveCollision(PHYS_joint *j1, PHYS_joint *j2, rex_int mass1, rex_int mass2, rex_int elasticity, rex_int friction, PHYS_closest_point_function env);

// Mostly for internal use, tests and potentially resolves a collision between
// a joint and environment, returns 0 if no collision happened, 1 if it happened
// and was resolved normally and 2 if it couldn't be resolved normally.
rex_ubyte PHYS_jointEnvironmentResolveCollision(PHYS_joint *joint, rex_int elasticity, rex_int friction, PHYS_closest_point_function env);

// Tests whether a body is currently colliding with the environment.//
rex_ubyte PHYS_bodyEnvironmentCollide(const PHYS_body *body, PHYS_closest_point_function env);

// Mostly for internal use, tests and potentially resolves a collision of a
// body with the environment, returns 1 if collision happened or 0 otherwise.
rex_ubyte PHYS_bodyEnvironmentResolveCollision(PHYS_body *body, PHYS_closest_point_function env);
rex_vec3i PHYS_bodyGetLinearVelocity(const PHYS_body *body);

// Computes the minimum bounding box of given body.
void PHYS_bodyGetAABB(const PHYS_body *body, rex_vec3i *vMin, rex_vec3i *vMax);

// Computes a bounding sphere of a body which is not minimal but faster to
// compute than the minimum bounding sphere.
void PHYS_bodyGetFastBSphere(const PHYS_body *body, rex_vec3i *center, rex_int *radius);

// Computes the minimum bounding sphere of a body (there is another function
// for a faster approximate bounding sphere).
void PHYS_bodyGetBSphere(const PHYS_body *body, rex_vec3i *center, rex_int *radius);
rex_ubyte PHYS_checkOverlapAABB(rex_vec3i v1Min, rex_vec3i v1Max, rex_vec3i v2Min, rex_vec3i v2Max);

// Mostly for internal use, checks and potentiall resolves collision of two
// bodies so as to keep them outside given environment. Returns 1 if collision
// happened or 0 otherwise.
rex_ubyte PHYS_bodiesResolveCollision(PHYS_body *b1, PHYS_body *b2, PHYS_closest_point_function env);

// Pins a joint of a body to specified location in space (sets its location and zeros its velocity).
void PHYS_jointPin(PHYS_joint *joint, rex_vec3i position);

// "Fakes" a rotation of a moving sphere by rotating it in the direction of
// its movement; this can create the illusion of the sphere actually rotating
// due to friction even if the physics sphere object (a body with a single joint)
// isn't rotating at all. Returns a rotation in the "about axis" format (see
// library conventions).
rex_vec3i PHYS_fakeSphereRotation(rex_vec3i position1, rex_vec3i position2, rex_int radius);

// Casts a ray against environment and returns the closest hit of a surface. If
// no surface was hit, a vector with all elements equal to PHYS_INFINITY will be
// returned. The function internally works differently for outside rays (rays
// cast from the outside of the environment) and inside rays. Outside rays can
// be traced with raymarching and will be processed very quickly and precisely;
// in this case if any intersection is found, the function will try to return a
// point outside (not guaranteed) the environment that's just in front of the hit
// surface. Inside rays are difficult and slow to trace because environment
// function won't provide distance, so the results aren't guaranteed to be
// precise (the ray may miss some intersections); here rays will be traced by
// given step (insideStepSize) and eventually iterated a bit towards the
// intersection -- if any intersection is found, the function will try to return
// a point inside (not guaranteed) the environment just before the hit
// surface.
rex_vec3i PHYS_castEnvironmentRay(rex_vec3i rayPos, rex_vec3i rayDir, PHYS_closest_point_function environment, rex_int insideStepSize, rex_int rayMarchMaxStep, rex_uint maxSteps);

// Casts a ray against bodies in a world (ignoring the environment), returns
// the position of the closest hit as well as the hit body's index in bodyIndex
// (unless the bodyIndex pointer is 0 in which case it is ignored). Similarly
// with jointIndex. If no hit is found a vector with all elements equal to
// PHYS_INFINITY will be returned and bodyIndex will be -1. A specific body can be
// excluded with excludeBody (negative value will just make this parameter
// ignored).
rex_vec3i PHYS_castBodyRay(rex_vec3i rayPos, rex_vec3i rayDir, rex_short excludeBody, const PHYS_world *world, rex_short *bodyIndex, rex_short *jointIndex);

// Performs one step (tick, frame, ...) of the physics world simulation
// including updating positions and velocities of bodies, collision detection and
// resolution, possible reshaping or deactivation of inactive bodies etc. The
// time length of the step is relative to all other units but it's ideal if it is
// 1/30th of a second.
void PHYS_worldStep(PHYS_world *world);
void PHYS_worldDeactivateAll(PHYS_world *world);
void PHYS_worldActivateAll(PHYS_world *world);
rex_int PHYS_worldGetNetSpeed(const PHYS_world *world);
rex_int PHYS_bodyGetNetSpeed(const PHYS_body *body);
rex_int PHYS_bodyGetAverageSpeed(const PHYS_body *body);
void PHYS_bodyMultiplyNetSpeed(PHYS_body *body, rex_int factor);
void PHYS_bodyLimitAverageSpeed(PHYS_body *body, rex_int speedMin, rex_int speedMax);

// Deactivates a body (puts it to sleep until another collision or force wake up).
void PHYS_bodyDeactivate(PHYS_body *body);
static inline rex_ubyte PHYS_bodyIsActive(const PHYS_body *body);

// Attempts to shift the joints of a soft body so that the tension of all
// springs becomes zero while keeping the joints near their current position.
// This function performs one iteration of the equalizing algorithm and doesn't
// guarantee a perfect solution, it may help to run multiple iterations (call
// this function multiple times).
void PHYS_bodyReshape(PHYS_body *body, PHYS_closest_point_function environment_function);

// Mostly for internal use, performs some "magic" on body connections, mainly
// cancelling out of velocities going against each other and also applying
// connection friction in soft bodies. The strong parameter indicates if the
// body is soft or not.
void PHYS_bodyCancelOutVelocities(PHYS_body *body, rex_ubyte strong);

// Moves a body by certain offset.
void PHYS_bodyMoveBy(PHYS_body *body, rex_vec3i offset);

// Moves a body (its center of mass) to given position.
void PHYS_bodyMoveTo(PHYS_body *body, rex_vec3i position);

// Zeros velocities of all soft body joints.
void PHYS_bodyStop(PHYS_body *body);
void PHYS_bodyActivate(PHYS_body *body);

// Adds velocity to a soft body.
void PHYS_bodyAccelerate(PHYS_body *body, rex_vec3i velocity);
void PHYS_bodyApplyGravity(PHYS_body *body, rex_int downwardsAccel);

// Adds angular velocity to a soft body. The rotation vector specifies the axis
// of rotation by its direction and angular velocity by its magnitude (magnitude
// of PHYS_FRACTIONS_PER_UNIT will add linear velocity of PHYS_FRACTIONS_PER_UNIT
// per tick to a point in the distance of PHYS_FRACTIONS_PER_UNIT from the
// rotation axis).
void PHYS_bodySpin(PHYS_body *body, rex_vec3i rotation);

// Same as PHYS_bodySpin but additionally allows to specify the center of the spin.
void PHYS_bodySpinWithCenter(PHYS_body *body, rex_vec3i rotation, rex_vec3i center);

// Instantly rotates a body about an axis (see library conventions for the rotation format).
void PHYS_bodyRotateByAxis(PHYS_body *body, rex_vec3i rotation);

// Computes the center of mass of a body. This averages the position of all
// joints; note that if you need, you may estimate the center of the body faster,
// e.g. by taking a position of a single "center joint", or averaging just 2
// extreme points.
rex_vec3i PHYS_bodyGetCenterOfMass(const PHYS_body *body);

// Draws a debug view of a 3D physics world using a provided pixel drawing
// function. This can be used to overlay a simple visualization of the physics
// objects to your main render, to spot exact borders of objects etc. The
// function draws simple dotted lines and circles with different "colors" for
// different types of objects (joints, connections, environemnt). camPos, camRot
// and camView should match the camera settings of your main renderer. CamView.x
// is horizontal resolution in pixels, camView.y is the vertical resolution,
// CamView.z says the camera focal length (~FOV) in rex_ints (0 means
// orthographic projection). envGridRes is the resolution of an environment probe
// grid (the function will probe points in space and draw borders of the physics
// environemnt), envGridSize is the size (int rex_ints) of the grid cell. Note
// the function may be slow (reducing envGridRes can help, workable value can be
// e.g. 16).
void PHYS_worldDebugDraw(PHYS_world *world, PHYS_debug_draw_function drawFunc, rex_vec3i camPos, rex_vec3i camRot, rex_vec3i camView, rex_ushort envGridRes, rex_int envGridSize);
rex_uint PHYS_jointHash(const PHYS_joint *joint);
rex_uint PHYS_connectionHash(const PHYS_connection *connection);
rex_uint PHYS_bodyHash(const PHYS_body *body);

// Computes 32 bit hash of the world, useful for checking if two states of the
// world differ. The function takes into account most of the relevant state but
// possibly not all of it, for details check the code.
rex_uint PHYS_worldHash(const PHYS_world *world);

// FUNCTIONS FOR GENERATING BODIES
void PHYS_makeBox(PHYS_joint joints[8], PHYS_connection connections[16], rex_int width, rex_int depth, rex_int height, rex_int jointSize);
void PHYS_makeCenterBox(PHYS_joint joints[9], PHYS_connection connections[18], rex_int width, rex_int depth, rex_int height, rex_int jointSize);
void PHYS_makeRect(PHYS_joint joints[4], PHYS_connection connections[6], rex_int width, rex_int depth, rex_int jointSize);
void PHYS_makeTriangle(PHYS_joint joints[3], PHYS_connection connections[3], rex_int sideLength, rex_int jointSize);
void PHYS_makeCenterRect(PHYS_joint joints[5], PHYS_connection connections[8], rex_int width, rex_int depth, rex_int jointSize);
void PHYS_makeCenterRectFull(PHYS_joint joints[5], PHYS_connection connections[10], rex_int width, rex_int depth, rex_int jointSize);
void PHYS_make2Line(PHYS_joint joints[2], PHYS_connection connections[1], rex_int length, rex_int jointSize);

// FUNCTIONS FOR BUILDING ENVIRONMENT
rex_vec3i PHYS_envAABoxInside(rex_vec3i point, rex_vec3i center, rex_vec3i size);
rex_vec3i PHYS_envAABox(rex_vec3i point, rex_vec3i center, rex_vec3i maxCornerVec);
rex_vec3i PHYS_envBox(rex_vec3i point, rex_vec3i center, rex_vec3i maxCornerVec, rex_vec3i rotation);
rex_vec3i PHYS_envSphere(rex_vec3i point, rex_vec3i center, rex_int radius);
rex_vec3i PHYS_envSphereInside(rex_vec3i point, rex_vec3i center, rex_int radius);
rex_vec3i PHYS_envHalfPlane(rex_vec3i point, rex_vec3i center, rex_vec3i normal);
rex_vec3i PHYS_envGround(rex_vec3i point, rex_int height);
rex_vec3i PHYS_envInfiniteCylinder(rex_vec3i point, rex_vec3i center, rex_vec3i direction, rex_int radius);
rex_vec3i PHYS_envCylinder(rex_vec3i point, rex_vec3i center, rex_vec3i direction, rex_int radius);
rex_vec3i PHYS_envCone(rex_vec3i point, rex_vec3i center, rex_vec3i direction, rex_int radius);
rex_vec3i PHYS_envLineSegment(rex_vec3i point, rex_vec3i a, rex_vec3i b);
rex_vec3i PHYS_envHeightmap(rex_vec3i point, rex_vec3i center, rex_int gridSize, rex_int (*heightFunction)(rex_int x, rex_int y), rex_int maxDist);

// Environment function for triagnular prism, e.g. for ramps. The sides array
// contains three 2D coordinates of points of the triangle in given plane with
// respect to the center. WARNING: the points must be specified in counter
// clowckwise direction! The direction var specified axis direction (0, 1 or 2).
rex_vec3i PHYS_envAATriPrism(rex_vec3i point, rex_vec3i center, const rex_int sides[6], rex_int depth, rex_ubyte direction);

// The following are helper macros for creating a union of shapes inside an
// environment function and accelerating them with bounding volumes.

#define PHYS_ENV_START(test,point) rex_vec3i _pBest = test, _pTest; \
	rex_int _dBest = PHYS_DISTANCE(_pBest,point), _dTest; \
	(void)(_pBest); (void)(_dBest); (void)(_dTest); (void)(_pTest); // supress war

#define PHYS_ENV_NEXT(test,point) \
	{ if (_pBest.x == point.x && _pBest.y == point.y && _pBest.z == point.z) \
	 return _pBest; \
	_pTest = test; _dTest = PHYS_DISTANCE(_pTest,point); \
	if (_dTest < _dBest) { _pBest = _pTest; _dBest = _dTest; } }

#define PHYS_ENV_END return _pBest;

#define PHYS_ENV_BCUBE_TEST(bodyBCubeC,bodyBCubeR,envBCubeC,envBCubeR) ( \
	(PHYS_abs(envBCubeC.x - bodyBCubeC.x) <= ((bodyBCubeR) + (envBCubeR))) && \
	(PHYS_abs(envBCubeC.y - bodyBCubeC.y) <= ((bodyBCubeR) + (envBCubeR))) && \
	(PHYS_abs(envBCubeC.z - bodyBCubeC.z) <= ((bodyBCubeR) + (envBCubeR))))

#define PHYS_ENV_BSPHERE_TEST(bodyBSphereC,bodyBSphereR,envBSphereC,envBSphereR)\
	(PHYS_DISTANCE(bodyBSphereC,envBSphereC) <= ((bodyBSphereR) + (envBSphereR)))
