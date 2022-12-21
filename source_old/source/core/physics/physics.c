// ========================================================
//
// FILE:			/source/core/physics/physics.c
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Rex Engine
//
// LICENSE:			ACSL 1.4
//
// DESCRIPTION:		Physics module.
//
// LAST EDITED:		October 29th, 2022
//
// ========================================================

// Include Rex Engine header
#include "rex.h"

rex_ushort _PHYS_body1Index, _PHYS_body2Index, _PHYS_joint1Index, _PHYS_joint2Index;
PHYS_collision_callback _PHYS_collision_callback;

static inline rex_int PHYS_nonZero(rex_int x)
{
	return x != 0 ? x : 1;
}

static inline rex_int PHYS_connectionTension(rex_int length,
	rex_int desiredLength)
{
	return (length * PHYS_F) / desiredLength
		- PHYS_F;
}

PHYS_joint PHYS_joint_calculate(rex_vec3i position, rex_int size)
{
	PHYS_joint result;

	result.velocity[0] = 0;
	result.velocity[1] = 0;
	result.velocity[2] = 0;
 
	result.position = position;

	size /= PHYS_JOINT_SIZE_MULTIPLIER;

	if (size > 0xff)
	{
		PHYS_LOG("WARNING: joint size too big in PHYS_joint");
	}

	result.radius = size;

	return result;
}

rex_vec3i PHYS_vec3(rex_int x, rex_int y, rex_int z)
{
	rex_vec3i r;

	r.x = x;
	r.y = y;
	r.z = z;

	return r;
}

rex_int PHYS_sqrt(rex_int x)
{
	rex_byte sign = 1;

	if (x < 0)
	{
		sign = -1;
		x *= -1;
	}

	rex_uint result = 0;
	rex_uint a = x;
	rex_uint b = 1u << 30;

	while (b > a)
		b >>= 2;

	while (b != 0)
	{
		if (a >= result + b)
		{
			a -= result + b;
			result = result +  2 * b;
		}

		b >>= 2;
		result >>= 1;
	}

	return result * sign;
}

rex_int PHYS_vec3Len(rex_vec3i v)
{
#define ANTI_OVERFLOW 25000
	if  (v.x < ANTI_OVERFLOW && v.x > -1 * ANTI_OVERFLOW &&
			 v.y < ANTI_OVERFLOW && v.y > -1 * ANTI_OVERFLOW &&
			 v.z < ANTI_OVERFLOW && v.z > -1 * ANTI_OVERFLOW)
	{
		return  PHYS_sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
	}
	else
	{
		v.x /= 32; v.y /= 32; v.z /= 32;
		return  PHYS_sqrt(v.x * v.x + v.y * v.y + v.z * v.z) * 32;
	}
#undef ANTI_OVERFLOW
}

rex_int PHYS_vec3LenApprox(rex_vec3i v)
{
	// 48 sided polyhedron approximation

	if (v.x < 0) v.x *= -1;
	if (v.y < 0) v.y *= -1;
	if (v.z < 0) v.z *= -1;
 
	if (v.x < v.y) // order the coordinates
	{
		if (v.x < v.z)
		{
			if (v.y < v.z)
			{ // v.x < v.y < v.z
				rex_int t = v.x; v.x = v.z; v.z = t;
			}
			else
			{ // v.x < v.z < v.y
				rex_int t = v.x; v.x = v.y; v.y = t;
				t = v.z; v.z = v.y; v.y = t;
			}
		}
		else
		{ // v.z < v.x < v.y
			rex_int t = v.x; v.x = v.y; v.y = t;
		}
	}
	else
	{
		if (v.y < v.z)
		{
			if (v.x < v.z)
			{ // v.y < v.x < v.z
				rex_int t = v.y; v.y = v.z; v.z = t;
				t = v.x; v.x = v.y; v.y = t;  
			}
			else
			{ // v.y < v.z < v.x
				rex_int t = v.y; v.y = v.z; v.z = t;
			}
		}
	}
		
	return (893 * v.x + 446 * v.y + 223 * v.z) / 1024;
}

rex_int PHYS_dist(rex_vec3i p1, rex_vec3i p2)
{
	p1 = PHYS_vec3Minus(p1,p2);
	return PHYS_vec3Len(p1); 
}

rex_int PHYS_distApprox(rex_vec3i p1, rex_vec3i p2)
{
	p1 = PHYS_vec3Minus(p1,p2);
	return PHYS_vec3LenApprox(p1); 
}

void PHYS_bodyInit(PHYS_body *body, 
	PHYS_joint *joints, rex_ubyte num_joints, 
	PHYS_connection *connections, rex_ubyte num_connections,
	rex_int mass)
{
	body->joints = joints;
	body->num_joints = num_joints;
	body->connections = connections;
	body->num_connections = num_connections;
	body->deactivate_count = 0;
	body->friction = PHYS_F / 2;
	body->elasticity = PHYS_F / 2;
	body->flags = 0;
	body->joint_mass = PHYS_nonZero(mass / num_joints);

	for (rex_uint i = 0; i < num_connections; ++i)
	{
		rex_int d = PHYS_DISTANCE(
			joints[connections[i].joint1].position,
			joints[connections[i].joint2].position);

		if (d > 0xffff)
		{
			PHYS_LOG("WARNING: joint distance too long in PHYS_bodyInit");
		}

		connections[i].length = d != 0 ? d : 1; // prevent later division by zero
	}
}

void PHYS_worldInit(PHYS_world *world, PHYS_body *bodies, rex_ushort num_bodies,
	PHYS_closest_point_function environment_function)
{
	world->bodies = bodies;
	world->num_bodies = num_bodies;
	world->environment_function = environment_function;
	world->collision_callback = 0;
}
	
#define C(n,a,b) connections[n].joint1 = a; connections[n].joint2 = b;

void PHYS_make2Line(PHYS_joint joints[2], PHYS_connection connections[1], rex_int length, rex_int jointSize)
{
	joints[0] = PHYS_joint_calculate(PHYS_vec3(length / 2,0,0),jointSize);
	joints[1] = PHYS_joint_calculate(PHYS_vec3(length / -2,0,0),jointSize);
	C(0, 0,1)
}

void PHYS_makeRect(PHYS_joint joints[4], PHYS_connection connections[6], rex_int width, rex_int depth,  rex_int jointSize)
{
	width /= 2;
	depth /= 2;

	for (rex_ubyte i = 0; i < 4; ++i)
		joints[i] = PHYS_joint_calculate(PHYS_vec3((i % 2) ? -1 * width : width,
			0,(i / 2) ? - 1 * depth : depth),jointSize);

	C(0, 0,1) C(1, 0,2) C (2, 3,1) C(3, 3,2)
	C(4, 0,3) C(5, 1,2)
}

void PHYS_makeCenterRect(PHYS_joint joints[5], PHYS_connection connections[8], rex_int width, rex_int depth, rex_int jointSize)
{
	PHYS_makeRect(joints,connections,width,depth,jointSize);

	joints[4] = PHYS_joint_calculate(PHYS_vec3(0,0,0),jointSize);

	C(6, 0,4) C(7, 3,4)
}

void PHYS_makeCenterRectFull(PHYS_joint joints[5], PHYS_connection connections[10], rex_int width, rex_int depth, rex_int jointSize)
{
	PHYS_makeCenterRect(joints,connections,width,depth,jointSize);
	C(8, 1,4) C(9, 2,4)
}

void PHYS_makeTriangle(PHYS_joint joints[3], PHYS_connection connections[3], rex_int sideLength, rex_int jointSize)
{
	joints[0] = PHYS_joint_calculate(PHYS_vec3(sideLength / 2,0,
		PHYS_sqrt((sideLength * sideLength) / 2) / 2),jointSize);

	joints[1] = joints[0];
	joints[1].position.x *= -1;
	joints[2] = PHYS_joint_calculate(PHYS_vec3(0,0,-1 * joints[0].position.z),jointSize);

	C(0, 0,1) C(1, 1,2) C(2, 2,0)
}

void PHYS_makeBox(PHYS_joint joints[8], PHYS_connection connections[16], rex_int width, rex_int depth, rex_int height, rex_int jointSize)
{
	width /= 2;
	depth /= 2;
	height /= 2;

	for (rex_ubyte i = 0; i < 8; ++i)
		joints[i] = PHYS_joint_calculate( 
			PHYS_vec3(  
				(i % 2) ? width : (-1 * width),
				((i >> 2) % 2) ? height : (-1 * height),
				((i >> 1) % 2) ? depth : (-1 * depth)),
			jointSize);

	C(0, 0,1) C(1, 1,3) C(2, 3,2) C(3, 2,0)  // top
	C(4, 4,5) C(5, 5,7) C(6, 7,6) C(7, 6,4)  // bottom
	C(8, 0,4) C(9, 1,5) C(10,3,7) C(11,2,6)  // middle
	C(12,0,7) C(13,1,6) C(14,2,5) C(15,3,4)  // diagonal
}

void PHYS_makeCenterBox(PHYS_joint joints[9], PHYS_connection connections[18], rex_int width, rex_int depth, rex_int height, rex_int jointSize)
{
	PHYS_makeBox(joints,connections,width,depth,height,jointSize);

	joints[8] = PHYS_joint_calculate(PHYS_vec3(0,0,0),jointSize);

	C(16, 0,8) C(17, 7,8)
}
	
#undef C

void PHYS_bodyDeactivate(PHYS_body *body)
{
	body->flags |= PHYS_BODY_FLAG_DEACTIVATED;
}

void PHYS_worldStep(PHYS_world *world)
{
	_PHYS_collision_callback = world->collision_callback;

	for (rex_ushort i = 0; i < world->num_bodies; ++i)
	{
		PHYS_body *body = world->bodies + i;   

		if (body->flags & (PHYS_BODY_FLAG_DEACTIVATED | PHYS_BODY_FLAG_DISABLED))
			continue; 

		PHYS_joint *joint = body->joints, *joint2;

		rex_vec3i origPos = body->joints[0].position;

		// apply velocities
		for (rex_ushort j = 0; j < body->num_joints; ++j)
		{
			// non-rotating bodies will copy the 1st joint's velocity
			if (body->flags & PHYS_BODY_FLAG_NONROTATING)
				for (rex_ubyte k = 0; k < 3; ++k)
					joint->velocity[k] = body->joints[0].velocity[k];

			joint->position.x += joint->velocity[0];
			joint->position.y += joint->velocity[1];
			joint->position.z += joint->velocity[2];

			joint++;
		}

		PHYS_connection *connection = body->connections;

		rex_vec3i aabbMin, aabbMax;

		PHYS_bodyGetAABB(body,&aabbMin,&aabbMax);
				
		_PHYS_body1Index = i;
	
		_PHYS_body2Index = _PHYS_body1Index;

		rex_ubyte collided =    
			PHYS_bodyEnvironmentResolveCollision(body,world->environment_function);

		if (body->flags & PHYS_BODY_FLAG_NONROTATING)
		{
			/* Non-rotating bodies may end up still colliding after environment coll 
			resolvement (unlike rotating bodies where each joint is ensured separately
			to not collide). So if still in collision, we try a few more times. If not
			successful, we simply undo any shifts we've done. This should absolutely
			prevent any body escaping out of environment bounds. */
 
			for (rex_ubyte i = 0; i < PHYS_NONROTATING_COLLISION_RESOLVE_ATTEMPTS; ++i) 
			{
				if (!collided)
					break;

				collided =
					PHYS_bodyEnvironmentResolveCollision(body,world->environment_function);
			}

			if (collided &&
				PHYS_bodyEnvironmentCollide(body,world->environment_function))
				PHYS_bodyMoveBy(body,PHYS_vec3Minus(origPos,body->joints[0].position));
		}
		else // normal, rotating bodies
		{
			rex_int bodyTension = 0;

			for (rex_ushort j = 0; j < body->num_connections; ++j) // joint tension
			{
				joint  = &(body->joints[connection->joint1]);
				joint2 = &(body->joints[connection->joint2]);

				rex_vec3i dir = PHYS_vec3Minus(joint2->position,joint->position);

				rex_int tension = PHYS_connectionTension(PHYS_LENGTH(dir),
					connection->length);

				bodyTension += tension > 0 ? tension : -tension;

				if (tension > PHYS_TENSION_ACCELERATION_THRESHOLD || 
					tension < -1 * PHYS_TENSION_ACCELERATION_THRESHOLD)
				{
					PHYS_vec3Normalize(&dir);

					if (tension > PHYS_TENSION_GREATER_ACCELERATION_THRESHOLD ||
						tension < -1 * PHYS_TENSION_GREATER_ACCELERATION_THRESHOLD)
					{ 
						/* apply twice the acceleration after a second threshold, not so
							 elegant but seems to work :) */
						dir.x *= 2;
						dir.y *= 2;
						dir.z *= 2;
					}

					dir.x /= PHYS_TENSION_ACCELERATION_DIVIDER;
					dir.y /= PHYS_TENSION_ACCELERATION_DIVIDER;
					dir.z /= PHYS_TENSION_ACCELERATION_DIVIDER;

					if (tension < 0)
					{
						dir.x *= -1;
						dir.y *= -1;
						dir.z *= -1;
					}

					joint->velocity[0] += dir.x;
					joint->velocity[1] += dir.y;
					joint->velocity[2] += dir.z;

					joint2->velocity[0] -= dir.x;
					joint2->velocity[1] -= dir.y;
					joint2->velocity[2] -= dir.z;
				}

				connection++;
			}

			if (body->num_connections > 0)
			{
				rex_ubyte hard = !(body->flags & PHYS_BODY_FLAG_SOFT);

				if (hard)
				{
					PHYS_bodyReshape(body,world->environment_function);

					bodyTension /= body->num_connections;
				
					if (bodyTension > PHYS_RESHAPE_TENSION_LIMIT)
						for (rex_ubyte k = 0; k < PHYS_RESHAPE_ITERATIONS; ++k)
							PHYS_bodyReshape(body,world->environment_function);
				}
				
				if (!(body->flags & PHYS_BODY_FLAG_SIMPLE_CONN))  
					PHYS_bodyCancelOutVelocities(body,hard);
			}
		}

		for (rex_ushort j = 0; j < world->num_bodies; ++j)
		{
			if (j > i || (world->bodies[j].flags & PHYS_BODY_FLAG_DEACTIVATED))
			{
				// firstly quick-check collision of body AA bounding boxes

				rex_vec3i aabbMin2, aabbMax2;
				PHYS_bodyGetAABB(&world->bodies[j],&aabbMin2,&aabbMax2);

				_PHYS_body2Index = j;

				if (PHYS_checkOverlapAABB(aabbMin,aabbMax,aabbMin2,aabbMax2) &&
					PHYS_bodiesResolveCollision(body,world->bodies + j,
					world->environment_function))
				{
					PHYS_bodyActivate(body);
					body->deactivate_count = PHYS_LIGHT_DEACTIVATION; 

					PHYS_bodyActivate(world->bodies + j);
					world->bodies[j].deactivate_count = PHYS_LIGHT_DEACTIVATION;
				}
			}
		}

		if (!(body->flags & PHYS_BODY_FLAG_ALWAYS_ACTIVE))
		{
			if (body->deactivate_count >= PHYS_DEACTIVATE_AFTER)
			{
				PHYS_bodyStop(body);
				body->deactivate_count = 0;
				body->flags |= PHYS_BODY_FLAG_DEACTIVATED;
			}
			else if (PHYS_bodyGetAverageSpeed(body) <= PHYS_LOW_SPEED)
				body->deactivate_count++;
			else
				body->deactivate_count = 0;
		}
	}
}

void PHYS_bodyActivate(PHYS_body *body)
{
	// the if check has to be here, don't remove it

	if (body->flags & PHYS_BODY_FLAG_DEACTIVATED)
	{
		PHYS_bodyStop(body);
		body->flags &= ~PHYS_BODY_FLAG_DEACTIVATED;
		body->deactivate_count = 0;
	}
}

rex_int PHYS_bodyGetNetSpeed(const PHYS_body *body)
{
#if PHYS_APPROXIMATE_NET_SPEED
	rex_vec3i netV = PHYS_vec3(0,0,0);
		
	const PHYS_joint *joint = body->joints;

	for (rex_ushort i = 0; i < body->num_joints; ++i)
	{
		netV.x += PHYS_abs(joint->velocity[0]);
		netV.y += PHYS_abs(joint->velocity[1]);
		netV.z += PHYS_abs(joint->velocity[2]);

		joint++;
	}

	return PHYS_vec3LenApprox(netV);
#else
	rex_int velocity = 0;

	const PHYS_joint *joint = body->joints;

	for (rex_ushort i = 0; i < body->num_joints; ++i)
	{
		velocity += PHYS_LENGTH(
			PHYS_vec3(joint->velocity[0],joint->velocity[1],joint->velocity[2]));

		joint++;
	}

	return velocity;
#endif
}

rex_int PHYS_bodyGetAverageSpeed(const PHYS_body *body)
{
	return PHYS_bodyGetNetSpeed(body) / body->num_joints;
}

void PHYS_bodyMultiplyNetSpeed(PHYS_body *body, rex_int factor)
{
	PHYS_joint *joint = body->joints;

	for (rex_ushort j = 0; j < body->num_joints; ++j)
	{
		for (rex_ubyte k = 0; k < 3; ++k)
			joint->velocity[k] = 
				(((rex_int) joint->velocity[k]) * factor) /
				PHYS_F;

		joint++;
	}
}

void PHYS_bodyLimitAverageSpeed(PHYS_body *body, rex_int speedMin,
	rex_int speedMax)
{
	for (rex_ubyte i = 0; i < 16; ++i)
	{
		rex_int speed = PHYS_bodyGetAverageSpeed(body);

		if (speed >= speedMin && speed <= speedMax)
			return;

		rex_int fraction =
			(((speedMax + speedMin) / 2) * PHYS_F) /
			PHYS_nonZero(speed);
		
		PHYS_bodyMultiplyNetSpeed(body,fraction);
	}
}

void PHYS_bodyCancelOutVelocities(PHYS_body *body, rex_ubyte strong)
{
	for (rex_ushort i = 0; i < body->num_connections; ++i)
	{
		PHYS_connection *c = &body->connections[i];

		PHYS_joint *j1 = &(body->joints[c->joint1]);
		PHYS_joint *j2 = &(body->joints[c->joint2]);
		
		rex_vec3i dir = PHYS_vec3Minus(j2->position,j1->position);

		rex_int len = PHYS_nonZero(PHYS_LENGTH(dir));

		rex_ubyte cancel = 1;

		if (strong)
		{
			rex_int tension = PHYS_connectionTension(len,c->length);

			cancel = tension <= PHYS_TENSION_ACCELERATION_THRESHOLD &&
				tension >= -1 * PHYS_TENSION_ACCELERATION_THRESHOLD;
		}

		if (cancel)
		{
			rex_vec3i
				v1 = PHYS_vec3(j1->velocity[0],j1->velocity[1],j1->velocity[2]),
				v2 = PHYS_vec3(j2->velocity[0],j2->velocity[1],j2->velocity[2]);

			dir.x = (dir.x * PHYS_F) / len; // normalize
			dir.y = (dir.y * PHYS_F) / len;
			dir.z = (dir.z * PHYS_F) / len;

			v1 = PHYS_vec3ProjectNormalized(v1,dir);
			v2 = PHYS_vec3ProjectNormalized(v2,dir);

			rex_vec3i avg = PHYS_vec3Plus(v1,v2);

			avg.x /= 2;
			avg.y /= 2;
			avg.z /= 2;

			if (strong)
			{
				j1->velocity[0] = j1->velocity[0] - v1.x + avg.x;
				j1->velocity[1] = j1->velocity[1] - v1.y + avg.y;
				j1->velocity[2] = j1->velocity[2] - v1.z + avg.z;

				j2->velocity[0] = j2->velocity[0] - v2.x + avg.x;
				j2->velocity[1] = j2->velocity[1] - v2.y + avg.y;
				j2->velocity[2] = j2->velocity[2] - v2.z + avg.z;
			}
			else
			{
				j1->velocity[0] = j1->velocity[0] - v1.x + (v1.x * 3 + avg.x) / 4;
				j1->velocity[1] = j1->velocity[1] - v1.y + (v1.y * 3 + avg.y) / 4;
				j1->velocity[2] = j1->velocity[2] - v1.z + (v1.z * 3 + avg.z) / 4;

				j2->velocity[0] = j2->velocity[0] - v2.x + (v2.x * 3 + avg.x) / 4;
				j2->velocity[1] = j2->velocity[1] - v2.y + (v2.y * 3 + avg.y) / 4;
				j2->velocity[2] = j2->velocity[2] - v2.z + (v2.z * 3 + avg.z) / 4;
			}
		}
	}
}

void PHYS_bodyReshape(PHYS_body *body, 
	PHYS_closest_point_function environment_function)
{
	for (rex_ushort i = 0; i < body->num_connections; ++i)
	{
		PHYS_connection *c = &body->connections[i];

		PHYS_joint *j1 = &(body->joints[c->joint1]);
		PHYS_joint *j2 = &(body->joints[c->joint2]);
			
		rex_vec3i dir = PHYS_vec3Minus(j2->position,j1->position);

		rex_vec3i middle = PHYS_vec3Plus(j1->position,j2->position);

		middle.x /= 2;
		middle.y /= 2;
		middle.z /= 2;

		PHYS_vec3Normalize(&dir);

		dir.x = (dir.x * c->length) / PHYS_F;
		dir.y = (dir.y * c->length) / PHYS_F;
		dir.z = (dir.z * c->length) / PHYS_F;

		rex_vec3i positionBackup = j1->position;

		j1->position.x = middle.x - dir.x / 2;
		j1->position.y = middle.y - dir.y / 2;
		j1->position.z = middle.z - dir.z / 2;

		if (environment_function != 0 && PHYS_LENGTH(PHYS_vec3Minus(j1->position,
			environment_function(j1->position,PHYS_JOINT_SIZE(*j1))))
			< PHYS_JOINT_SIZE(*j1))
			j1->position = positionBackup;
	
		positionBackup = j2->position;

		j2->position.x = j1->position.x + dir.x;
		j2->position.y = j1->position.y + dir.y;
		j2->position.z = j1->position.z + dir.z; 

		if (environment_function != 0 && PHYS_LENGTH(PHYS_vec3Minus(j2->position,
			environment_function(j2->position,PHYS_JOINT_SIZE(*j2))))
			< PHYS_JOINT_SIZE(*j2))
			j2->position = positionBackup;
	}
}

rex_vec3i PHYS_vec3Plus(rex_vec3i v1, rex_vec3i v2)
{
	v1.x += v2.x;
	v1.y += v2.y;
	v1.z += v2.z;

	return v1;
}

rex_vec3i PHYS_vec3Minus(rex_vec3i v1, rex_vec3i v2)
{
	v1.x -= v2.x;
	v1.y -= v2.y;
	v1.z -= v2.z;

	return v1;
}

void PHYS_vec3Normalize(rex_vec3i *v)
{
	rex_int l = PHYS_LENGTH(*v);

	if (l == 0)
		*v = PHYS_vec3(PHYS_F,0,0);
	else
	{
		if (l < 16) // too short vec would cause inacurracte normalization
		{
			v->x *= 8;
			v->y *= 8;
			v->z *= 8;
			l = PHYS_LENGTH(*v);
		}

		v->x = (v->x * PHYS_F) / l;
		v->y = (v->y * PHYS_F) / l;
		v->z = (v->z * PHYS_F) / l;
	}
}

rex_vec3i PHYS_bodyGetRotation(const PHYS_body *body, rex_ushort joint1, 
	rex_ushort joint2, rex_ushort joint3)
{
	return PHYS_rotationFromVecs(
		PHYS_vec3Minus(
			body->joints[joint2].position,
			body->joints[joint1].position),
		PHYS_vec3Minus(
			body->joints[joint3].position,
			body->joints[joint1].position));
}

rex_vec3i PHYS_bodyGetCenterOfMass(const PHYS_body *body)
{
	// note that joint sizes don't play a role as all weight the same

	rex_vec3i result = PHYS_vec3(0,0,0);

	const PHYS_joint *j = body->joints;

	for (rex_ushort i = 0; i < body->num_joints; ++i)
	{
		result = PHYS_vec3Plus(result,j->position);
		j++;
	}

	result.x /= body->num_joints;
	result.y /= body->num_joints;
	result.z /= body->num_joints;
 
	return result;
}

void PHYS_bodySpinWithCenter(PHYS_body *body, rex_vec3i rotation, rex_vec3i center)
{
	for (rex_ushort i = 0; i < body->num_joints; ++i)
	{
		PHYS_joint *j = body->joints + i;

		rex_vec3i toPoint = PHYS_vec3Minus(j->position,center);

		toPoint = PHYS_vec3Project(toPoint,rotation);
		toPoint = PHYS_vec3Plus(center,toPoint);
		toPoint = PHYS_vec3Minus(j->position,toPoint);
		toPoint = PHYS_vec3Cross(toPoint,rotation);

		j->velocity[0] += toPoint.x;
		j->velocity[1] += toPoint.y;
		j->velocity[2] += toPoint.z;
	}
}

void PHYS_bodySpin(PHYS_body *body, rex_vec3i rotation)
{
	PHYS_bodySpinWithCenter(body,rotation,PHYS_bodyGetCenterOfMass(body));
}

rex_vec3i _PHYS_rotateByAxis(rex_vec3i p, rex_vec3i axisNormalized, rex_int angle)
{
	rex_vec3i projected = PHYS_vec3ProjectNormalized(p,axisNormalized);

	rex_vec3i a = PHYS_vec3Minus(p,projected);

	if (a.x == 0 && a.y == 0 && a.z == 0)
		return p;

	rex_vec3i b = PHYS_vec3Cross(a,axisNormalized);

	return PHYS_vec3Plus(projected,PHYS_vec3Plus(
		PHYS_vec3Times(a,PHYS_cos(angle)),
		PHYS_vec3Times(b,PHYS_sin(angle))));
}

void PHYS_bodyRotateByAxis(PHYS_body *body, rex_vec3i rotation)
{
	rex_vec3i bodyCenter = PHYS_bodyGetCenterOfMass(body);
	rex_int angle = PHYS_LENGTH(rotation);

	PHYS_vec3Normalize(&rotation);

	for (rex_ushort i = 0; i < body->num_joints; ++i)
	{
		rex_vec3i toPoint = PHYS_vec3Minus(body->joints[i].position,bodyCenter);
		body->joints[i].position = PHYS_vec3Plus(bodyCenter,
		_PHYS_rotateByAxis(toPoint,rotation,angle));
	}
}

rex_vec3i PHYS_vec3Cross(rex_vec3i v1, rex_vec3i v2)
{
	rex_vec3i r;

	r.x = (v1.y * v2.z - v1.z * v2.y) / PHYS_F;
	r.y = (v1.z * v2.x - v1.x * v2.z) / PHYS_F;
	r.z = (v1.x * v2.y - v1.y * v2.x) / PHYS_F;

	return r;
}

rex_vec3i PHYS_vec3ProjectNormalized(rex_vec3i v, rex_vec3i baseNormalized)
{
	rex_vec3i r;

	rex_int p = PHYS_vec3Dot(v,baseNormalized);

	r.x = (p * baseNormalized.x) / PHYS_F;
	r.y = (p * baseNormalized.y) / PHYS_F;
	r.z = (p * baseNormalized.z) / PHYS_F;
	
	return r;
}

rex_vec3i PHYS_vec3Project(rex_vec3i v, rex_vec3i base)
{
	PHYS_vec3Normalize(&base);

	return PHYS_vec3ProjectNormalized(v,base);
}

void PHYS_bodyMoveBy(PHYS_body *body, rex_vec3i offset)
{
	for (rex_ushort i = 0; i < body->num_joints; ++i)
		body->joints[i].position = PHYS_vec3Plus(body->joints[i].position,
			offset);
}

void PHYS_bodyApplyGravity(PHYS_body *body, rex_int downwardsAccel)
{
	if ((body->flags & PHYS_BODY_FLAG_DEACTIVATED) ||
			(body->flags & PHYS_BODY_FLAG_DISABLED))
		return;

	for (rex_ushort i = 0; i < body->num_joints; ++i)
		body->joints[i].velocity[1] -= downwardsAccel;
}

void PHYS_bodyAccelerate(PHYS_body *body, rex_vec3i velocity)
{
	PHYS_bodyActivate(body);

	for (rex_ushort i = 0; i < body->num_joints; ++i)
	{
		body->joints[i].velocity[0] += velocity.x;
		body->joints[i].velocity[1] += velocity.y;
		body->joints[i].velocity[2] += velocity.z;
	}
}

void PHYS_bodyStop(PHYS_body *body)
{
	for (rex_ushort i = 0; i < body->num_joints; ++i)
	{
		body->joints[i].velocity[0] = 0;
		body->joints[i].velocity[1] = 0;
		body->joints[i].velocity[2] = 0;
	}
}

void _PHYS_bodyNonrotatingJointCollided(PHYS_body *b, rex_short jointIndex, 
	rex_vec3i origPos, rex_ubyte success)
{
	origPos = PHYS_vec3Minus(b->joints[jointIndex].position,origPos);

	for (rex_ushort i = 0; i < b->num_joints; ++i)
		if (i != jointIndex)
		{
			b->joints[i].position = PHYS_vec3Plus(b->joints[i].position,origPos);
		 
			if (success) 
				for (rex_ubyte j = 0; j < 3; ++j)
					b->joints[i].velocity[j] = b->joints[jointIndex].velocity[j];
		}
}

rex_int PHYS_vec3Dot(rex_vec3i v1, rex_vec3i v2)
{
	return (v1.x * v2.x + v1.y * v2.y + v1.z * v2.z) / PHYS_F;
}

rex_int PHYS_cos(rex_int x)  
{
	return PHYS_sin(x + PHYS_F / 4);
}

rex_int PHYS_sin(rex_int x)  
{
	rex_byte sign = 1;
		
	if (x < 0) // odd function
	{
		x *= -1;
		sign = -1;
	}
		
	x %= PHYS_F;
	
	if (x > PHYS_F / 2)
	{
		x -= PHYS_F / 2;
		sign *= -1;
	}

	rex_int tmp = PHYS_F - 2 * x;
 
	#define _PI2 5053 // 9.8696044 * PHYS_F
	return sign * // Bhaskara's approximation
		(((32 * x * _PI2) / PHYS_F) * tmp) / 
		((_PI2 * (5 * PHYS_F - (8 * x * tmp) / 
			PHYS_F)) / PHYS_F);
	#undef _PI2
}

rex_ubyte PHYS_bodiesResolveCollision(PHYS_body *b1, PHYS_body *b2,
	PHYS_closest_point_function env)
{
	rex_ubyte r = 0;

	for (rex_ushort i = 0; i < b1->num_joints; ++i)
		for (rex_ushort j = 0; j < b2->num_joints; ++j)
		{
			rex_vec3i origPos2 = b2->joints[j].position;
			rex_vec3i origPos1 = b1->joints[i].position;

			_PHYS_joint1Index = i;
			_PHYS_joint2Index = j;

			if (PHYS_jointsResolveCollision(&(b1->joints[i]),&(b2->joints[j]),
				b1->joint_mass,b2->joint_mass,(b1->elasticity + b2->elasticity) / 2,
				(b1->friction + b2->friction) / 2,env))
			{
				r = 1;

				if (b1->flags & PHYS_BODY_FLAG_NONROTATING)
					_PHYS_bodyNonrotatingJointCollided(b1,i,origPos1,1);

				if (b2->flags & PHYS_BODY_FLAG_NONROTATING)
					_PHYS_bodyNonrotatingJointCollided(b2,j,origPos2,1);
			}
		}

	return r;
}

rex_ubyte PHYS_jointsResolveCollision(PHYS_joint *j1, PHYS_joint *j2,
	rex_int mass1, rex_int mass2, rex_int elasticity, rex_int friction,
	PHYS_closest_point_function env)
{
	rex_vec3i dir = PHYS_vec3Minus(j2->position,j1->position);

	rex_int d = PHYS_LENGTH(dir) - PHYS_JOINT_SIZE(*j1) - PHYS_JOINT_SIZE(*j2);

	if (d < 0) // collision?
	{
		if (_PHYS_collision_callback != 0 && !_PHYS_collision_callback(
				_PHYS_body1Index,_PHYS_joint1Index,_PHYS_body2Index,_PHYS_joint2Index,
				PHYS_vec3Plus(j1->position,dir)))
			return 0;

		rex_vec3i
			pos1Backup = j1->position,
			pos2Backup = j2->position;
	
		// separate joints, the shift distance will depend on the weight ratio:

		d = -1 * d + PHYS_COLLISION_RESOLUTION_MARGIN;

		PHYS_vec3Normalize(&dir);

		rex_int ratio = (mass2 * PHYS_F) / 
			PHYS_nonZero(mass1 + mass2);

		rex_int shiftDistance = (ratio * d) / PHYS_F;

		rex_vec3i shift = PHYS_vec3Times(dir,shiftDistance);

		j1->position = PHYS_vec3Minus(j1->position,shift);

		shiftDistance = d - shiftDistance;

		shift = PHYS_vec3Times(dir,shiftDistance);

		j2->position = PHYS_vec3Plus(j2->position,shift);

		// compute new velocities:

		rex_int v1, v2;

		rex_vec3i vel = PHYS_vec3(j1->velocity[0],j1->velocity[1],j1->velocity[2]);

		vel = PHYS_vec3Project(vel,dir);

		j1->velocity[0] = j1->velocity[0] - vel.x;
		j1->velocity[1] = j1->velocity[1] - vel.y;
		j1->velocity[2] = j1->velocity[2] - vel.z;

		/* friction explanation: Not physically correct (doesn't depend on load), 
		friction basically means we weighted average the velocities of the bodies
		in the direction perpendicular to the hit normal, in the ratio of their
		masses, friction coefficient just says how much of this effect we apply
		(it multiplies the friction vectors we are subtracting) */

		rex_vec3i frictionVec =
			PHYS_vec3(j1->velocity[0],j1->velocity[1],j1->velocity[2]);

		v1 = PHYS_vec3Dot(vel,dir);
		vel = PHYS_vec3(j2->velocity[0],j2->velocity[1],j2->velocity[2]);
		vel = PHYS_vec3Project(vel,dir);

		j2->velocity[0] = j2->velocity[0] - vel.x;
		j2->velocity[1] = j2->velocity[1] - vel.y;
		j2->velocity[2] = j2->velocity[2] - vel.z;

		frictionVec = PHYS_vec3Minus(
			PHYS_vec3(j2->velocity[0],j2->velocity[1],j2->velocity[2]),
			frictionVec);

		v2 = PHYS_vec3Dot(vel,dir);

		PHYS_getVelocitiesAfterCollision(&v1,&v2,mass1,mass2,elasticity);

		vel = PHYS_vec3Times(dir,v1);

#define assignVec(j, i, d, o) j->velocity[i] = j->velocity[i] + vel.d o (((frictionVec.d * ratio) / PHYS_F) * friction) / PHYS_F;

		assignVec(j1,0,x,+)
		assignVec(j1,1,y,+)
		assignVec(j1,2,z,+)

		vel = PHYS_vec3Times(dir,v2);

		ratio = PHYS_F - ratio;

		assignVec(j2,0,x,-)
		assignVec(j2,1,y,-)
		assignVec(j2,2,z,-)

#undef assignVec

		if (env != 0)
		{
			// ensure the joints aren't colliding with environment

			if (PHYS_jointEnvironmentResolveCollision(j1,elasticity,friction,env) == 2)
				j1->position = pos1Backup;

			if (PHYS_jointEnvironmentResolveCollision(j2,elasticity,friction,env) == 2)
				j2->position = pos2Backup;
		}

		return 1;
	}

	return 0;
}

rex_vec3i PHYS_vec3Times(rex_vec3i v, rex_int units)
{
	v.x = (v.x * units) / PHYS_F;
	v.y = (v.y * units) / PHYS_F;
	v.z = (v.z * units) / PHYS_F;

	return v;
}

rex_vec3i PHYS_vec3TimesPlain(rex_vec3i v, rex_int q)
{
	v.x *= q;
	v.y *= q;
	v.z *= q;

	return v;
}

void PHYS_getVelocitiesAfterCollision(rex_int *v1, rex_int *v2,
	rex_int m1, rex_int m2, rex_int elasticity)
{
	/* In the following a lot of PHYS_F cancel out, feel free to
		 check if confused. */

	rex_int m1Pm2 = PHYS_nonZero(m1 + m2);
	rex_int v2Mv1 = PHYS_nonZero(*v2 - *v1);

	rex_int m1v1Pm2v2 = ((m1 * *v1) + (m2 * *v2));

	*v1 = (((elasticity * m2 / PHYS_F) * v2Mv1)
		+ m1v1Pm2v2) / m1Pm2;

	*v2 = (((elasticity * m1 / PHYS_F) * -1 * v2Mv1)
		+ m1v1Pm2v2) / m1Pm2;
}

rex_ubyte PHYS_jointEnvironmentResolveCollision(PHYS_joint *joint,
	rex_int elasticity, rex_int friction, PHYS_closest_point_function env)
{
	rex_vec3i toJoint =
		PHYS_vec3Minus(joint->position,env(joint->position,PHYS_JOINT_SIZE(*joint)));

	rex_int len = PHYS_LENGTH(toJoint);

	if (len <= PHYS_JOINT_SIZE(*joint))
	{
		if (_PHYS_collision_callback != 0)
			if (!_PHYS_collision_callback(_PHYS_body1Index,
				_PHYS_joint1Index,_PHYS_body2Index,_PHYS_joint2Index,
				PHYS_vec3Minus(joint->position,toJoint)))
				return 0;

		// colliding

		rex_vec3i positionBackup = joint->position, shift;
		rex_ubyte success = 0;

		if (len > 0)
		{
			/* Joint center is still outside the geometry so we can determine the
				 normal and use it to shift it outside. This can still leave the joint
				 colliding though, so try to repeat it a few times. */

			for (int i = 0; i < PHYS_COLLISION_RESOLUTION_ITERATIONS; ++i)
			{
				shift = toJoint;

				PHYS_vec3Normalize(&shift); 

				shift = PHYS_vec3Times(shift,PHYS_JOINT_SIZE(*joint) - len + 
					PHYS_COLLISION_RESOLUTION_MARGIN);
					
				joint->position = PHYS_vec3Plus(joint->position,shift);
	
				toJoint = PHYS_vec3Minus(joint->position,env(joint->position,
					PHYS_JOINT_SIZE(*joint)));

				len = PHYS_LENGTH(toJoint); // still colliding?

				if (len >= PHYS_JOINT_SIZE(*joint))
				{
					success = 1;
					break;
				}
			}
		}

		if (!success)
		{
			/* Shifting along normal was unsuccessfull, now try different approach:
				 shift back by joint velocity. */

			shift = PHYS_vec3(-1 * joint->velocity[0],-1 * joint->velocity[1],
				-1 * joint->velocity[2]);
			
			for (int i = 0; i < PHYS_COLLISION_RESOLUTION_ITERATIONS; ++i)
			{
				joint->position = PHYS_vec3Plus(joint->position,shift);

				toJoint = PHYS_vec3Minus(joint->position,
					env(joint->position,PHYS_JOINT_SIZE(*joint)));

				len = PHYS_LENGTH(toJoint); // still colliding?

				if (len >= PHYS_JOINT_SIZE(*joint))
				{
					success = 1;
					break;
				}

				shift.x /= 2; // decrease the step a bit
				shift.y /= 2;
				shift.z /= 2;
			}
		}

		if (success)
		{
			rex_vec3i vel = PHYS_vec3(joint->velocity[0],joint->velocity[1],
				joint->velocity[2]);

			vel = PHYS_vec3Project(vel,shift); // parallel part of velocity

			rex_vec3i vel2 = PHYS_vec3Minus( // perpendicular part of velocity
				PHYS_vec3(joint->velocity[0],joint->velocity[1],joint->velocity[2]),vel);

			vel2 = PHYS_vec3Times(vel2,friction);

			vel = PHYS_vec3Times(vel,PHYS_F + elasticity);

			joint->velocity[0] -= vel.x + vel2.x;
			joint->velocity[1] -= vel.y + vel2.y;
			joint->velocity[2] -= vel.z + vel2.z;
		}
		else
		{
			PHYS_LOG("WARNING: joint-environment collision couldn't be resolved");

			joint->position = positionBackup;
			joint->velocity[0] = 0;
			joint->velocity[1] = 0;
			joint->velocity[2] = 0;

			return 2;
		}

		return 1;
	}

	return 0;
}

rex_ubyte PHYS_bodyEnvironmentCollide(const PHYS_body *body,
	PHYS_closest_point_function env)
{
	for (rex_ushort i = 0; i < body->num_joints; ++i)
	{
		const PHYS_joint *joint = body->joints + i;

		rex_int size = PHYS_JOINT_SIZE(*joint);

		if (PHYS_DISTANCE(joint->position,env(joint->position,size)) <= size)
			return 1;
	}

	return 0;
}

void PHYS_bodyGetFastBSphere(const PHYS_body *body, rex_vec3i *center, rex_int *radius)
{
	rex_vec3i b;

	PHYS_bodyGetAABB(body,center,&b);

	center->x = (center->x + b.x) / 2;
	center->y = (center->y + b.y) / 2;
	center->z = (center->z + b.z) / 2;

	*radius = PHYS_DISTANCE(*center,b);
}

void PHYS_bodyGetBSphere(const PHYS_body *body, rex_vec3i *center, rex_int *radius)
{
	*radius = PHYS_INFINITY;
	*center = PHYS_bodyGetCenterOfMass(body);

	const PHYS_joint *j = body->joints;

	for (rex_ushort i = 0; i < body->num_joints; ++i)
	{
		rex_vec3i diff;

		rex_int js = PHYS_JOINT_SIZE(*j);

		/* Sadly we have to have these conditions here which slow this down. If we
			 were only computing a BB sphere of a point cloud, we wouldn't have to
			 compute abs vals (as squaring would effectively compute them), but here
			 we need to add joint size which needs to know about the sign. */

		diff.x = ((center->x > j->position.x) ?
			(center->x - j->position.x) : (j->position.x - center->x)) + js;

		diff.y = ((center->y > j->position.y) ?
			(center->y - j->position.y) : (j->position.y - center->y)) + js;

		diff.z = ((center->z > j->position.z) ?
			(center->z - j->position.z) : (j->position.z - center->z)) + js;

		rex_int distSquared = 
			diff.x * diff.x + diff.y * diff.y + diff.z * diff.z;

		if (distSquared < *radius)
			*radius = distSquared;

		j++;
	}

	*radius = PHYS_sqrt(*radius);
}

rex_ubyte PHYS_bodyEnvironmentResolveCollision(PHYS_body *body, PHYS_closest_point_function env)
{
	rex_vec3i c;
	rex_int d;

	PHYS_bodyGetFastBSphere(body,&c,&d);

	if (PHYS_DISTANCE(c,env(c,d)) > d)
		return 0;

	// now test the full body collision:

	rex_ubyte collision = 0;

	for (rex_ushort i = 0; i < body->num_joints; ++i)
	{
		rex_vec3i previousPos = body->joints[i].position;

		_PHYS_joint1Index = i;

		rex_ubyte r = PHYS_jointEnvironmentResolveCollision(
			body->joints + i,body->elasticity,body->friction,env);

		if (r)
		{
			collision = 1;

			if (body->flags & PHYS_BODY_FLAG_NONROTATING)
				_PHYS_bodyNonrotatingJointCollided(body,i,previousPos,r == 1);
		}
	}

	return collision;
}

rex_vec3i PHYS_vec3Normalized(rex_vec3i v)
{
	PHYS_vec3Normalize(&v);
	return v;
}

rex_int PHYS_atan(rex_int x)
{
	/* atan approximation by polynomial 
		 WARNING: this will break with different value of PHYS_FRACTIONS_PER_UNIT */

	rex_int sign = 1, x2 = x * x;

	if (x < 0)
	{
		x *= -1;
		sign = -1;
	}

	if (x > 30000) // anti overflow
		return sign * (PHYS_F / 4);

	return sign *
		(307 * x + x2) / ((267026 + 633 * x + x2) / 128);
}

void _PHYS_vec2Rotate(rex_int *x, rex_int *y, rex_int angle)
{
	rex_int tmp = *x;

	rex_int s = PHYS_sin(angle);
	rex_int c = PHYS_cos(angle);

	*x = (c * *x - s * *y) / PHYS_F;
	*y = (s * tmp + c * *y) / PHYS_F;
}

rex_int PHYS_vec2Angle(rex_int x, rex_int y)
{
	rex_int r = 0;

	if (x != 0)
	{
		r = PHYS_atan((y * PHYS_F) / x);

		if (x < 0)
			r += PHYS_F / 2;
		else if (r < 0)
			r += PHYS_F;
	}
	else
	{
		if (y < 0)
			r = (3 * PHYS_F) / 4;
		else if (y > 0)
			r = PHYS_F / 4;
		// else (y == 0) r stays 0
	}

	return r;
}

rex_vec3i PHYS_rotationFromVecs(rex_vec3i forward, rex_vec3i right)
{
	rex_vec3i result;

	// get rotation around Y:

	result.y = PHYS_vec2Angle(forward.z,-1 * forward.x);

	// now rotate back by this angle to align with x = 0 plane:

	_PHYS_vec2Rotate(&forward.z,&forward.x,result.y);
	_PHYS_vec2Rotate(&right.z,&right.x,result.y);

	// now do the same for the second axis:

	result.x = 
		PHYS_vec2Angle(forward.z,forward.y);

	_PHYS_vec2Rotate(&right.z,&right.y,-1 * result.x);

	result.z = PHYS_vec2Angle(right.x,-1 * right.y);

	return result;
}

rex_vec3i _PHYS_project3DPoint(rex_vec3i p, rex_vec3i camPos, rex_vec3i camRot, rex_vec3i camView)
{
	// transform to camera space:

	p = PHYS_vec3Minus(p,camPos);

	_PHYS_vec2Rotate(&p.z,&p.x,camRot.y);
	_PHYS_vec2Rotate(&p.z,&p.y,-1 * camRot.x);
	_PHYS_vec2Rotate(&p.y,&p.x,-1 * camRot.z);

	if (p.z <= 0)
		return p;

	if (camView.z != 0)
	{
		// perspective

		p.x = (p.x * camView.z) / p.z;
		p.y = (p.y * camView.z) / p.z;

		p.x = camView.x / 2 + (p.x * camView.x) / (2 * PHYS_F);
		p.y = camView.y / 2 - (p.y * camView.x) / (2 * PHYS_F);
																			// ^ x here intentional
	}
	else
	{
		// ortho

		p.x = camView.x / 2 + p.x;
		p.y = camView.y / 2 - p.y;
	}

	return p;
}

void _PHYS_drawDebugPixel(rex_int x, rex_int y, rex_int w, rex_int h, rex_ubyte c, PHYS_debug_draw_function f)
{
	if (x >= 0 && x < w && y >= 0 && y < h)
		f(x,y,c);
}

void PHYS_worldDebugDraw(PHYS_world *world, PHYS_debug_draw_function drawFunc, rex_vec3i camPos, rex_vec3i camRot, rex_vec3i camView, rex_ushort envGridRes, rex_int envGridSize)
{
#define Z_LIMIT 250
	if (world->environment_function != 0)
	{
		// environment:

		rex_vec3i testPoint;

		rex_int gridHalfSize = (envGridSize * envGridRes) / 2;

		rex_vec3i center;

		if (envGridRes != 0)
		{
			center = PHYS_vec3(0,PHYS_sin(camRot.x),PHYS_cos(camRot.x));

			_PHYS_vec2Rotate(&center.x,&center.z,camRot.y);

			center = PHYS_vec3Times(center,gridHalfSize);
			center = PHYS_vec3Plus(camPos,center);

			center.x = (center.x / envGridSize) * envGridSize;
			center.y = (center.y / envGridSize) * envGridSize;
			center.z = (center.z / envGridSize) * envGridSize;
		}

		testPoint.y = center.y - gridHalfSize;

		for (rex_ubyte j = 0; j < envGridRes; ++j)
		{
			testPoint.x = center.x - gridHalfSize;

			for (rex_ubyte k = 0; k < envGridRes; ++k)
			{
				testPoint.z = center.z - gridHalfSize;

				for (rex_ubyte l = 0; l < envGridRes; ++l)
				{
					rex_vec3i r = world->environment_function(testPoint,envGridSize);

					if (r.x != testPoint.x || r.y != testPoint.y || r.z != testPoint.z)
					{
						r = _PHYS_project3DPoint(r,camPos,camRot,camView);
 
						if (r.z > Z_LIMIT)
							_PHYS_drawDebugPixel(r.x,r.y,camView.x,camView.y,
								PHYS_DEBUG_COLOR_ENVIRONMENT,drawFunc);
					}

					testPoint.z += envGridSize;
				}

				testPoint.x += envGridSize;
			}

			testPoint.y += envGridSize;
		}
	}

	for (rex_ushort i = 0; i < world->num_bodies; ++i)
	{
		// connections:
		for (rex_ushort j = 0; j < world->bodies[i].num_connections; ++j)
		{
			rex_vec3i
				p1 = world->bodies[i].joints[
					world->bodies[i].connections[j].joint1].position,
				p2 = world->bodies[i].joints[
					world->bodies[i].connections[j].joint2].position;

			p1 = _PHYS_project3DPoint(p1,camPos,camRot,camView);
			p2 = _PHYS_project3DPoint(p2,camPos,camRot,camView);

			if (p1.z <= Z_LIMIT || p2.z <= Z_LIMIT)
				continue;

			rex_vec3i diff = PHYS_vec3Minus(p2,p1);

#define SEGS 16

			rex_ubyte c = (world->bodies[i].flags & PHYS_BODY_FLAG_DEACTIVATED) ?
				PHYS_DEBUG_COLOR_INACTIVE : PHYS_DEBUG_COLOR_CONNECTION;

			for (rex_ushort k = 0; k < SEGS; ++k)
			{
				p2.x = p1.x + (diff.x * k) / SEGS;
				p2.y = p1.y + (diff.y * k) / SEGS;

				_PHYS_drawDebugPixel(p2.x,p2.y,camView.x,camView.y,c,drawFunc);
			}
#undef SEGS
		}

		// joints:
		for (rex_ushort j = 0; j < world->bodies[i].num_joints; ++j)
		{
			rex_vec3i p = _PHYS_project3DPoint(world->bodies[i].joints[j].position,
				camPos,camRot,camView);

			if (p.z > Z_LIMIT)
			{
				rex_ubyte color = (world->bodies[i].flags & PHYS_BODY_FLAG_DEACTIVATED) ?
					PHYS_DEBUG_COLOR_INACTIVE : PHYS_DEBUG_COLOR_JOINT;

				_PHYS_drawDebugPixel(p.x,p.y,camView.x,camView.y,color,drawFunc);

				rex_int size = PHYS_JOINT_SIZE(world->bodies[i].joints[j]);

				if (camView.z != 0) // not ortho?
				{
					size /= 2;
					size = (size * camView.x) / PHYS_F;
					size = (size * camView.z) / p.z;
				}

#define SEGS 4
				for (rex_ubyte k = 0; k < SEGS + 1; ++k)
				{
					rex_int 
						dx = (PHYS_sin(PHYS_F * k / (8 * SEGS)) * size)
							/ PHYS_F,
						dy = (PHYS_cos(PHYS_F * k / (8 * SEGS)) * size)
							/ PHYS_F;

#define dp(a,b,c,d) \
	_PHYS_drawDebugPixel(p.x a b,p.y c d,camView.x,camView.y,color,drawFunc);
					dp(+,dx,+,dy) dp(+,dx,-,dy) dp(-,dx,+,dy) dp(-,dx,-,dy)
					dp(+,dy,+,dx) dp(+,dy,-,dx) dp(-,dy,+,dx) dp(-,dy,-,dx)
#undef dp
#undef SEGS
				}
			}
		}
	}
#undef Z_LIMIT
}

rex_vec3i PHYS_envBox(rex_vec3i point, rex_vec3i center, rex_vec3i maxCornerVec, rex_vec3i rotation)
{
	point = PHYS_pointRotate(PHYS_vec3Minus(point,center),
		PHYS_rotationInverse(rotation));

	return PHYS_vec3Plus(center,PHYS_pointRotate(PHYS_envAABox(point,PHYS_vec3(0,0,0),
		maxCornerVec),rotation));
}

rex_vec3i PHYS_envAABox(rex_vec3i point, rex_vec3i center, rex_vec3i maxCornerVec)
{
	rex_vec3i shifted = PHYS_vec3Minus(point,center);
	rex_byte sign[3] = {1, 1, 1};

	if (shifted.x < 0)
	{
		shifted.x *= -1;
		sign[0] = -1;
	}

	if (shifted.y < 0)
	{
		shifted.y *= -1;
		sign[1] = -1;
	}

	if (shifted.z < 0)
	{
		shifted.z *= -1;
		sign[2] = -1;
	}

	rex_ubyte region =
		(shifted.x > maxCornerVec.x) |
		((shifted.y > maxCornerVec.y) << 1) |
		((shifted.z > maxCornerVec.z) << 2);

	switch (region)
	{
#define align(c,i) point.c = center.c + sign[i] * maxCornerVec.c

		case 0x01: align(x,0); break;
		case 0x02: align(y,1); break;
		case 0x04: align(z,2); break;

		case 0x03: align(x,0); align(y,1); break;
		case 0x05: align(x,0); align(z,2); break;
		case 0x06: align(y,1); align(z,2); break;

		case 0x07: align(x,0); align(y,1); align(z,2); break; 
		default: break;

#undef align
	}

	return point;
}

rex_vec3i PHYS_envAABoxInside(rex_vec3i point, rex_vec3i center, rex_vec3i size)
{
	size.x /= 2;
	size.y /= 2;
	size.z /= 2;

	rex_vec3i shifted = PHYS_vec3Minus(point,center);

	rex_vec3i a = PHYS_vec3Minus(size,shifted),
					 b = PHYS_vec3Plus(shifted,size);

	rex_byte sx = 1, sy = 1, sz = 1;

	if (b.x < a.x)
	{
		a.x = b.x;
		sx = -1;
	}

	if (b.y < a.y)
	{
		a.y = b.y;
		sy = -1;
	}

	if (b.z < a.z)
	{
		a.z = b.z;
		sz = -1;
	}

	if (a.x < 0 || a.y < 0 || a.z < 0)
		return point;

	if (a.x < a.y)
	{
		if (a.x < a.z)
			point.x = center.x + sx * size.x;
		else
			point.z = center.z + sz * size.z;
	}
	else
	{
		if (a.y < a.z)
			point.y = center.y + sy * size.y;
		else
			point.z = center.z + sz * size.z;
	}

	return point;
}

rex_vec3i PHYS_envSphereInside(rex_vec3i point, rex_vec3i center, rex_int radius)
{
	rex_vec3i shifted = PHYS_vec3Minus(point,center);

	rex_int l = PHYS_LENGTH(shifted);

	if (l >= radius)
		return point;
	else if (l < 0)
		return PHYS_vec3(center.x + radius,center.y,center.z);

	PHYS_vec3Normalize(&shifted);
 
	return PHYS_vec3Plus(center,PHYS_vec3Times(shifted,radius));
}

rex_vec3i PHYS_envSphere(rex_vec3i point, rex_vec3i center, rex_int radius)
{
	rex_vec3i dir = PHYS_vec3Minus(point,center);

	rex_int l = PHYS_LENGTH(dir);

	if (l <= radius)
		return point;

	dir.x = (dir.x * radius) / l;
	dir.y = (dir.y * radius) / l;
	dir.z = (dir.z * radius) / l;

	return PHYS_vec3Plus(center,dir);
}

rex_vec3i PHYS_envHalfPlane(rex_vec3i point, rex_vec3i center, rex_vec3i normal)
{
	rex_vec3i point2 = PHYS_vec3Minus(point,center);

	rex_int tmp =
		point2.x * normal.x + point2.y * normal.y + point2.z * normal.z;

	if (tmp < 0)
		return point;

	rex_int l = PHYS_LENGTH(normal);

	tmp /= l;

	normal.x = (normal.x * PHYS_F) / l;
	normal.y = (normal.y * PHYS_F) / l;
	normal.z = (normal.z * PHYS_F) / l;

	return PHYS_vec3Minus(point,
		PHYS_vec3Times(normal,tmp));
}

rex_ubyte PHYS_checkOverlapAABB(rex_vec3i v1Min, rex_vec3i v1Max, rex_vec3i v2Min, rex_vec3i v2Max)
{
	rex_int dist;

#define test(c) \
	dist = v1Min.c + v1Max.c - v2Max.c - v2Min.c; \
	if (dist < 0) dist *= -1; \
	if (dist > v1Max.c - v1Min.c + v2Max.c - v2Min.c) return 0;

	test(x)
	test(y)
	test(z)

#undef test

	return 1;
}

void PHYS_bodyGetAABB(const PHYS_body *body, rex_vec3i *vMin, rex_vec3i *vMax)
{
	*vMin = body->joints[0].position;
	*vMax = *vMin;

	rex_int js = PHYS_JOINT_SIZE(body->joints[0]);

	vMin->x -= js;
	vMin->y -= js;
	vMin->z -= js;

	vMax->x += js;
	vMax->y += js;
	vMax->z += js;

	for (rex_ushort i = 1; i < body->num_joints; ++i)
	{
		rex_int v;
	
		js = PHYS_JOINT_SIZE(body->joints[i]);
	
#define test(c) \
	v = body->joints[i].position.c - js; \
	if (v < vMin->c) \
		vMin->c = v; \
	v += 2 * js; \
	if (v > vMax->c) \
		vMax->c = v;

		test(x)
		test(y)
		test(z)

#undef test
	}
}

void PHYS_jointPin(PHYS_joint *joint, rex_vec3i position)
{
	joint->position = position;
	joint->velocity[0] = 0;
	joint->velocity[1] = 0;
	joint->velocity[2] = 0;
}

rex_vec3i PHYS_pointRotate(rex_vec3i point, rex_vec3i rotation)
{
	_PHYS_vec2Rotate(&point.y,&point.x,rotation.z);
	_PHYS_vec2Rotate(&point.z,&point.y,rotation.x);
	_PHYS_vec2Rotate(&point.x,&point.z,rotation.y);

	return point;
}

rex_vec3i PHYS_rotationInverse(rex_vec3i rotation)
{
	/* If r1 = (X,Y,Z) is rotation in convention ABC then r1^-1 = (-X,-Y,-Z) in
		 convention CBA is its inverse rotation. We exploit this, i.e. we rotate
		 forward/right vectors in opposite axis order and then turn the result
		 into normal rotation/orientation. */

	rex_vec3i f = PHYS_vec3(0,0,PHYS_F);
	rex_vec3i r = PHYS_vec3(PHYS_F,0,0);

	rotation.x *= -1;
	rotation.y *= -1;
	rotation.z *= -1;

	_PHYS_vec2Rotate(&f.x,&f.z,rotation.y);
	_PHYS_vec2Rotate(&f.z,&f.y,rotation.x);
	_PHYS_vec2Rotate(&f.y,&f.x,rotation.z);

	_PHYS_vec2Rotate(&r.x,&r.z,rotation.y);
	_PHYS_vec2Rotate(&r.z,&r.y,rotation.x);
	_PHYS_vec2Rotate(&r.y,&r.x,rotation.z);

	return PHYS_rotationFromVecs(f,r);
}

rex_vec3i PHYS_rotationRotateByAxis(rex_vec3i rotation, rex_vec3i rotationByAxis)
{
	rex_vec3i f = PHYS_pointRotate(PHYS_vec3(0,0,PHYS_F),rotation);
	rex_vec3i r = PHYS_pointRotate(PHYS_vec3(PHYS_F,0,0),rotation);

	rex_int a = PHYS_LENGTH(rotationByAxis);
	PHYS_vec3Normalize(&rotationByAxis);

	f = _PHYS_rotateByAxis(f,rotationByAxis,a);
	r = _PHYS_rotateByAxis(r,rotationByAxis,a);

	return PHYS_rotationFromVecs(f,r);
}

rex_int PHYS_keepInRange(rex_int x, rex_int xMin, rex_int xMax)
{
	return x > xMin ? (x < xMax ? x : xMax) : xMin;
}

rex_vec3i PHYS_vec3KeepWithinDistanceBand(rex_vec3i point, rex_vec3i center,
	rex_int minDistance, rex_int maxDistance)
{
	rex_vec3i toPoint = PHYS_vec3Minus(point,center);

	rex_int l = PHYS_LENGTH(toPoint);

	if (l <= maxDistance)
	{
		if (l >= minDistance)
			return point;
 
		l = minDistance;
	}
	else
		l = maxDistance;

	return PHYS_vec3Plus(center,
		PHYS_vec3Times(PHYS_vec3Normalized(toPoint),l));
}

rex_vec3i PHYS_vec3KeepWithinBox(rex_vec3i point, rex_vec3i boxCenter, rex_vec3i boxMaxVect)
{
	point.x = PHYS_keepInRange(point.x,
		boxCenter.x - boxMaxVect.x,boxCenter.x + boxMaxVect.x);

	point.y = PHYS_keepInRange(point.y,
		boxCenter.y - boxMaxVect.y,boxCenter.y + boxMaxVect.y);

	point.z = PHYS_keepInRange(point.z,
		boxCenter.z - boxMaxVect.z,boxCenter.z + boxMaxVect.z);

	return point;
}

rex_vec3i PHYS_envInfiniteCylinder(rex_vec3i point, rex_vec3i center, rex_vec3i direction, rex_int radius)
{
	rex_vec3i d = PHYS_vec3Minus(point,center);
	d = PHYS_vec3Minus(d,PHYS_vec3Project(d,direction));

	rex_int l = PHYS_LENGTH(d);

	if (l <= radius)
		return point;

	radius = l - radius;

	d.x = (d.x * radius) / l;
	d.y = (d.y * radius) / l;
	d.z = (d.z * radius) / l;
		
	return PHYS_vec3Minus(point,d);
}

rex_vec3i PHYS_envCylinder(rex_vec3i point, rex_vec3i center, rex_vec3i direction, rex_int radius)
{
	point = PHYS_vec3Minus(point,center);

	rex_vec3i projected = PHYS_vec3Project(point,direction);

	point = PHYS_envInfiniteCylinder(point,PHYS_vec3(0,0,0),direction,radius);

	rex_int lDir = PHYS_nonZero(PHYS_LENGTH(direction));

	rex_int lDiff = PHYS_LENGTH(projected) - lDir;

	if (lDiff > 0)
	{
		direction.x = (direction.x * lDiff) / lDir;
		direction.y = (direction.y * lDiff) / lDir;
		direction.z = (direction.z * lDiff) / lDir;

		point = (PHYS_vec3Dot(projected,direction)) >= 0 ?
			PHYS_vec3Minus(point,direction) : PHYS_vec3Plus(point,direction);
	}

	return PHYS_vec3Plus(center,point);
}

rex_vec3i PHYS_fakeSphereRotation(rex_vec3i position1, rex_vec3i position2, rex_int radius)
{
	rex_vec3i m;

	m.x = position1.z - position2.z;
	m.y = 0;
	m.z = position2.x - position1.x;
		
	rex_int l = PHYS_sqrt(m.x * m.x + m.z * m.z);

	if (l == 0)
		return PHYS_vec3(0,0,0);

	rex_int d = (PHYS_DISTANCE(position1,position2) * 
		PHYS_F) / (radius * 4);

	m.x = (m.x * d) / l;
	m.z = (m.z * d) / l;
	
	return m;
}

rex_vec3i PHYS_castEnvironmentRay(rex_vec3i rayPos, rex_vec3i rayDir, PHYS_closest_point_function environment, rex_int insideStepSize, rex_int rayMarchMaxStep, rex_uint maxSteps)
{
	rex_vec3i p = rayPos;
	rex_vec3i p2 = environment(rayPos,rayMarchMaxStep);
	rex_int totalD = 0;

	PHYS_vec3Normalize(&rayDir);

	rex_ubyte found = 0; // 0 = nothing found, 1 = out/in found, 2 = in/out found

	if (p2.x != p.x || p2.y != p.y || p2.z != p.z)
	{
		// outside ray: ray march

		for (rex_uint i = 0; i < maxSteps; ++i)
		{
			rex_int d = PHYS_DISTANCE(p,p2);

			if (d > rayMarchMaxStep)
				d = rayMarchMaxStep;

			totalD += d;

			p2 = PHYS_vec3Plus(rayPos,PHYS_vec3Times(rayDir,totalD));

			if (d == 0 || 
				(p2.x == p.x && p2.y == p.y && p2.z == p.z))
				return p2; // point not inside env but dist == 0, ideal case

			rex_vec3i pTest = environment(p2,rayMarchMaxStep);

			if (pTest.x == p2.x && pTest.y == p2.y && pTest.z == p2.z)
			{
				// stepped into env, will have to iterate
				found = 1;
				break;
			}

			p = p2;
			p2 = pTest;
		}
	}
	else if (insideStepSize != 0)
	{
		// inside ray: iterate by fixed steps

		for (rex_uint i = 0; i < maxSteps; ++i)
		{
			totalD += insideStepSize;

			p2 = PHYS_vec3Plus(rayPos,PHYS_vec3Times(rayDir,totalD));

			rex_vec3i pTest = environment(p2,16);

			if (p2.x != pTest.x || p2.y != pTest.y || p2.z != pTest.z)
			{
				found = 2;
				break;
			}

			p = p2;
			p2 = pTest;
		}
	}

	if (found)
	{
		/* Here we've found two points (p, p2), each one the other side of the
			 env surface. Now iterate (binary search) to find the exact surface
			 pos. */

		for (rex_ubyte i = 0; i < 128; ++i) // upper limit just in case
		{
			rex_vec3i middle = PHYS_vec3Plus(p,p2);
 
			middle.x /= 2;
			middle.y /= 2;
			middle.z /= 2;

			if ((middle.x == p.x && middle.y == p.y && middle.z == p.z) ||
				(middle.x == p2.x && middle.y == p2.y && middle.z == p2.z))
				break; // points basically next to each other, don't continue

			rex_vec3i pTest = environment(middle,16); // 16: just a small number

			if ((found == 1) ==
				(pTest.x == middle.x && pTest.y == middle.y && pTest.z == middle.z))
				p2 = middle;
			else
				p = middle;
		}

		return (found == 1) ? p : p2;
	}

	return PHYS_vec3(PHYS_INFINITY,PHYS_INFINITY,PHYS_INFINITY);
}

rex_vec3i PHYS_castBodyRay(rex_vec3i rayPos, rex_vec3i rayDir, rex_short excludeBody, const PHYS_world *world, rex_short *bodyIndex, rex_short *jointIndex)
{
	rex_vec3i bestP = PHYS_vec3(PHYS_INFINITY,PHYS_INFINITY,PHYS_INFINITY);
	rex_int bestD = PHYS_INFINITY;

	if (bodyIndex != 0)
		*bodyIndex = -1;

	if (jointIndex != 0)
		*jointIndex = -1;

	PHYS_vec3Normalize(&rayDir);

	for (rex_ushort i = 0; i < world->num_bodies; ++i)
	{
		rex_vec3i c, p;
		rex_int r, d;

		PHYS_bodyGetFastBSphere(&world->bodies[i],&c,&r);

		c = PHYS_vec3Minus(c,rayPos);
		p = PHYS_vec3ProjectNormalized(c,rayDir);

		if (PHYS_vec3Dot(p,rayDir) >= 0) // point is in ray's forward dir?
		{
			d = PHYS_DISTANCE(p,c);

			if (d <= r)
			{
				// bounding sphere hit, now check all joints:

				const PHYS_joint *joint = world->bodies[i].joints;

				for (rex_ushort j = 0; j < world->bodies[i].num_joints; ++j)
				{
					c = joint->position;
					c = PHYS_vec3Minus(c,rayPos);
					p = PHYS_vec3ProjectNormalized(c,rayDir);

					if (PHYS_vec3Dot(p,rayDir) >= 0)
					{
						d = PHYS_DISTANCE(p,c);
						rex_int js = PHYS_JOINT_SIZE(*joint);

						if (d <= js)
						{
							// joint hit, compute exact coordinates:

							if (bodyIndex != 0)
								*bodyIndex = i;

							if (jointIndex != 0)
								*jointIndex = j;

							c = PHYS_vec3Times(rayDir,PHYS_sqrt(js * js - d * d));
							// ^ offset vector to two intersections
							p = PHYS_vec3Plus(p,rayPos);

							rex_vec3i
								i1 = PHYS_vec3Plus(p,c), // intersection points
								i2 = PHYS_vec3Minus(p,c);

							d = PHYS_DISTANCE(rayPos,i1);
							rex_int d2 = PHYS_DISTANCE(rayPos,i2);

							if (d2 < d) // take the closer one
							{
								d = d2;
								i1 = i2;
							}

							if (d < bestD)
							{
								bestD = d;
								bestP = i1;
							}
						}
					}

					joint++;
				}
			}
		}
	}

	return bestP;
}

void PHYS_worldDeactivateAll(PHYS_world *world)
{
	for (rex_ushort i = 0; i < world->num_bodies; ++i)
		PHYS_bodyDeactivate(&world->bodies[i]);
}

void PHYS_worldActivateAll(PHYS_world *world)
{
	for (rex_ushort i = 0; i < world->num_bodies; ++i)
		PHYS_bodyActivate(&world->bodies[i]);
}

rex_int PHYS_worldGetNetSpeed(const PHYS_world *world)
{
	rex_int result = 0;

	for (rex_ushort i = 0; i < world->num_bodies; ++i)
		result += PHYS_bodyGetNetSpeed(world->bodies + i);

	return result;
}

rex_vec3i PHYS_bodyGetLinearVelocity(const PHYS_body *body)
{
	rex_vec3i r = PHYS_vec3(0,0,0);

	for (rex_ushort i = 0; i < body->num_joints; ++i)
	{
		rex_short *v = body->joints[i].velocity;
		r = PHYS_vec3Plus(r,PHYS_vec3(v[0],v[1],v[2]));
	}

	r.x /= body->num_joints;
	r.y /= body->num_joints;
	r.z /= body->num_joints;
		
	return r;
}

rex_int PHYS_abs(rex_int x)
{
	return x >= 0 ? x : (-1 * x);
}

rex_int PHYS_max(rex_int a, rex_int b)
{
	return (a > b) ? a : b;
}

rex_int PHYS_min(rex_int a, rex_int b)
{
	return (a < b) ? a : b;
}

rex_vec3i PHYS_envAATriPrism(rex_vec3i point, rex_vec3i center,
	const rex_int sides[6], rex_int depth, rex_ubyte direction)
{
	point = PHYS_vec3Minus(point,center);

	if (direction == 1)
	{
		rex_int tmp = point.z;
		point.z = point.y;
		point.y = tmp;
	}
	else if (direction == 2)
	{
		rex_int tmp = point.z;
		point.z = point.x;
		point.x = tmp;
	}

	depth /= 2;

	if (point.z > depth)
		point.z = depth;
	else
	{
		depth *= -1;

		if (point.z < depth)
			point.z = depth;
	}

	for (rex_ubyte i = 0; i < 6; i += 2)
	{
		rex_ubyte i2 = i < 4 ? i + 2 : 0;

		rex_vec3i p =
			PHYS_envHalfPlane(point,PHYS_vec3(sides[i],sides[i + 1],0),
			PHYS_vec3(sides[i2 + 1] - sides[i + 1],sides[i] - sides[i2],0));

		if (p.x != point.x || p.y != point.y)
		{
			point = p;

			if ( // dot product to determine which side the point is on 
				(sides[i2] - sides[i]) * (point.x - sides[i]) +
				(sides[i2 + 1] - sides[i + 1]) * (point.y - sides[i + 1]) < 0)
			{
				point.x = sides[i]; point.y = sides[i + 1];
			}
			else if ( // same but for the other vertex
				(sides[i] - sides[i2]) * (point.x - sides[i2]) +
				(sides[i + 1] - sides[i2 + 1]) * (point.y - sides[i2 + 1]) < 0)
			{
				point.x = sides[i2]; point.y = sides[i2 + 1];
			}

			break;
		}
	}

	if (direction == 1)
	{
		rex_int tmp = point.z;
		point.z = point.y;
		point.y = tmp;
	}
	else if (direction == 2)
	{
		rex_int tmp = point.z;
		point.z = point.x;
		point.x = tmp;
	}

	return PHYS_vec3Plus(point,center);
}

rex_vec3i PHYS_envGround(rex_vec3i point, rex_int height)
{
	if (point.y > height)
		point.y = height;

	return point;
}

rex_uint _PHYS_hash(rex_uint n)
{
	// parameters found by hash-prospector project
	n = 250009959 * (n ^ (n >> 17));
	n = 2626308659 * (n ^ (n >> 15));
	return n ^ (n >> 16);
}

rex_uint PHYS_jointHash(const PHYS_joint *joint)
{
	rex_uint 
		r = _PHYS_hash(joint->position.x);
		r = _PHYS_hash(r ^ joint->position.y);
		r = _PHYS_hash(r ^ joint->position.z);
		r = _PHYS_hash(r ^
			(((rex_uint) joint->velocity[0]) |
			(((rex_uint) joint->velocity[1]) << 16)));
		r = _PHYS_hash(r ^ 
			(((rex_uint) joint->velocity[2]) |
			((rex_uint) joint->radius)));
	
	return r;
}

rex_uint PHYS_connectionHash(const PHYS_connection *connection)
{
	return _PHYS_hash(
	 ((rex_uint) connection->length) |
	 (((rex_uint) connection->joint1) << 16) |
	 (((rex_uint) connection->joint2) << 24));
}

rex_uint PHYS_bodyHash(const PHYS_body *body)
{
	rex_uint r = _PHYS_hash(
		((rex_uint) body->joint_mass) |
		(((rex_uint) body->flags) << 16) |
		(((rex_uint) body->deactivate_count) << 24)) ^
			_PHYS_hash(
		((rex_uint) body->friction) |
		(((rex_uint) body->elasticity) << 16));

	for (rex_ubyte i = 0; i < body->num_joints; ++i)
		r = _PHYS_hash(r ^ PHYS_jointHash(&body->joints[i]));

	for (rex_ubyte i = 0; i < body->num_connections; ++i)
		r = _PHYS_hash(r ^ PHYS_connectionHash(&body->connections[i]));

	return r;
}

rex_uint PHYS_worldHash(const PHYS_world *world)
{
	rex_uint r = 0;

	for (rex_ubyte i = 0; i < world->num_bodies; ++i)
		r = _PHYS_hash(r ^ PHYS_bodyHash(&world->bodies[i]));

	return r;
}

void PHYS_bodyMoveTo(PHYS_body *body, rex_vec3i position)
{
	position = PHYS_vec3Minus(position,PHYS_bodyGetCenterOfMass(body));

	for (rex_ubyte i = 0; i < body->num_joints; ++i)
		body->joints[i].position = PHYS_vec3Plus(body->joints[i].position,position);
}

rex_ubyte PHYS_testClosestPointFunction(PHYS_closest_point_function f,
	rex_vec3i cornerFrom, rex_vec3i cornerTo, rex_ubyte gridResolution,
	rex_short allowedError, rex_vec3i *errorPoint)
{
	rex_vec3i p;

	cornerTo = PHYS_vec3Minus(cornerTo,cornerFrom);

	for (rex_ushort z = 0; z < gridResolution; ++z)
	{
		p.z = cornerFrom.z + (z * cornerTo.z) / gridResolution;

		for (rex_ushort y = 0; y < gridResolution; ++y)
		{
			p.y = cornerFrom.y + (y * cornerTo.y) / gridResolution;

			for (rex_ushort x = 0; x < gridResolution; ++x)
			{
				p.x = cornerFrom.x + (x * cornerTo.x) / gridResolution;

				rex_vec3i p2 = f(p,PHYS_INFINITY);

				if (p.x != p2.x || p.y != p2.y || p.z != p2.z) // only test outside
				{
					// 1st try to approach the closest point and see if it stays the same:

					rex_vec3i p3 = p;

					for (rex_ubyte i = 0; i < 3; ++i)
					{
						p3 =
							PHYS_vec3((p3.x + p2.x) / 2,(p3.y + p2.y) / 2,(p3.z + p2.z) / 2);

						rex_vec3i p4 = f(p3,PHYS_INFINITY);

						if (PHYS_abs(p4.x - p2.x) + PHYS_abs(p4.y - p2.y) 
							+ PHYS_abs(p4.z - p2.z) > allowedError) // taxicab dist. for speed
						{
							if (errorPoint != 0)
								*errorPoint = p;

							return 0;
						}
					}
 
					// now test 8 points inside the sphere of radius:

					rex_int d = PHYS_DISTANCE(p,p2);

					p3.z = p.z - d / 2;
				 
					for (rex_ubyte zz = 0; zz < 2; ++zz)
					{
						p3.y = p.y - d / 2;

						for (rex_ubyte yy = 0; yy < 2; ++yy)
						{
							p3.x = p.x - d / 2;

							for (rex_ubyte zz = 0; zz < 2; ++zz)
							{
								if (PHYS_DISTANCE(p,f(p3,PHYS_INFINITY)) + allowedError < d)
								{
									/* In the sphere of distance radius to the original point's
										 closest point we've gotten a closer point which should
										 never happen. */

									if (errorPoint != 0)
										*errorPoint = p;

									return 0;
								}

								p3.x += d;
							}

							p3.y += d;
						}
 
						p3.z += d;
					}
				}
			}
		}
	}

	return 1;
}

rex_vec3i PHYS_envLineSegment(rex_vec3i point, rex_vec3i a, rex_vec3i b)
{
	point = PHYS_vec3Minus(point,a);

	b = PHYS_vec3Minus(b,a);

	point = PHYS_vec3Project(point,b);

	if (PHYS_vec3Dot(point,b) < 0)
		point = PHYS_vec3(0,0,0);
	else if (PHYS_abs(point.x) + PHYS_abs(point.y) + PHYS_abs(point.z) >
		PHYS_abs(b.x) + PHYS_abs(b.y) + PHYS_abs(b.z))
		point = b;

	point = PHYS_vec3Plus(point,a);

	return point;
}

rex_vec3i PHYS_envHeightmap(rex_vec3i point, rex_vec3i center, rex_int gridSize,
	rex_int (*heightFunction)(rex_int x, rex_int y), rex_int maxDist)
{
	point = PHYS_vec3Minus(point,center);

	rex_vec3i closestP = PHYS_vec3(PHYS_INFINITY,PHYS_INFINITY,PHYS_INFINITY);
	rex_int closestD = PHYS_INFINITY;

	rex_short startSquareX = point.x / gridSize - (point.x < 0),
					startSquareY = point.z / gridSize - (point.z < 0);

	rex_short squareX = startSquareX,
					squareY = startSquareY;

	rex_ubyte spiralDir = 1;
	rex_ushort spiralStep = 1, spiralStepsLeft = 1;

	rex_vec3i // 4 corners of the current square
		bl = PHYS_vec3(squareX * gridSize,heightFunction(squareX,squareY),
			squareY * gridSize),
		br = PHYS_vec3(bl.x + gridSize,heightFunction(squareX + 1,squareY),bl.z),
		tl = PHYS_vec3(bl.x,heightFunction(squareX,squareY + 1),bl.z + gridSize),
		tr = PHYS_vec3(br.x,heightFunction(squareX + 1,squareY + 1),tl.z);

	for (rex_ushort i = 0; i < 1024; ++i) // while (1) should work in theory but...
	{
		if ((PHYS_min(PHYS_abs(squareX - startSquareX),
			PHYS_abs(squareY - startSquareY)) - 1) * gridSize
			> PHYS_min(maxDist,closestD))
			break; // here we can no longer find the dist we're looking for => end

		for (rex_ubyte j = 0; j < 2; ++j) // check the two triangles of the segment
		{
			rex_vec3i testP = PHYS_envHalfPlane(point,j == 0 ? bl : tr,
				PHYS_vec3Normalized(j == 0 ?
					PHYS_vec3Cross(PHYS_vec3Minus(tl,bl),PHYS_vec3Minus(br,bl)) :
					PHYS_vec3Cross(PHYS_vec3Minus(br,tr),PHYS_vec3Minus(tl,tr))));

			rex_int testD = PHYS_DISTANCE(testP,point);

			if (testD < closestD)
			{
				if (j == 0 ? // point is inside the triangle?
					(testP.x >= bl.x && testP.z >= bl.z &&
						(testP.x - bl.x <= tl.z - testP.z)) :
					(testP.x <= tr.x && testP.z <= tr.z &&
						(testP.x - bl.x >= tl.z - testP.z)))
				{
					closestP = testP;
					closestD = testD;
				}
				else
				{
					// point outside the triangle, check individual boundary sides
#define testEdge(a,b) \
	testP = PHYS_envLineSegment(point,a,b); testD = PHYS_DISTANCE(testP,point); \
	if (testD < closestD) { closestP = testP; closestD = testD; }

					testEdge(j == 0 ? bl : tr,br)
					testEdge(j == 0 ? bl : tr,tl)
					testEdge(br,tl)

#undef testEdge
				}
			}
		}

		// now step to another square, in spiralling way:

		switch (spiralDir)
		{
			case 0: // moving up
				squareY++; 

				bl = tl; br = tr;
				tl = PHYS_vec3(bl.x,heightFunction(squareX,squareY + 1),bl.z + gridSize);
				tr = PHYS_vec3(br.x,heightFunction(squareX + 1,squareY + 1),bl.z 
					+ gridSize);

				break;

			case 1: // moving right
				squareX++;

				bl = br; tl = tr;
				tr = PHYS_vec3(tl.x + gridSize,heightFunction(squareX + 1,squareY + 1),
					tl.z);
				br = PHYS_vec3(bl.x + gridSize,heightFunction(squareX + 1,squareY),bl.z);

				break;

			case 2: // moving down
				squareY--;

				tl = bl; tr = br;
				bl = PHYS_vec3(tl.x,heightFunction(squareX,squareY),tl.z - gridSize);
				br = PHYS_vec3(tr.x,heightFunction(squareX + 1,squareY),tr.z - gridSize);

				break;

			case 3: // moving left
				squareX--;

				br = bl; tr = tl;
				tl = PHYS_vec3(tr.x - gridSize,heightFunction(squareX,squareY + 1),tr.z);
				bl = PHYS_vec3(br.x - gridSize,heightFunction(squareX,squareY),br.z);

				break;

			default: break;
		}

		spiralStepsLeft--;

		if (spiralStepsLeft == 0)
		{
			spiralDir = spiralDir != 0 ? spiralDir - 1 : 3;

			if (spiralDir == 3 || spiralDir == 1)
				spiralStep++;

			spiralStepsLeft = spiralStep;
		}
	}

	return PHYS_vec3Plus(closestP,center);
}

rex_vec3i PHYS_envCone(rex_vec3i point, rex_vec3i center, rex_vec3i direction,
	rex_int radius)
{
	point = PHYS_vec3Minus(point,center);

	if (PHYS_vec3Dot(point,direction) <= 0)
	{
		// underneath the cone

		direction.x *= -1;
		direction.y *= -1;
		direction.z *= -1;

		point = PHYS_envHalfPlane(point,PHYS_vec3(0,0,0),direction);

		rex_int dist = PHYS_LENGTH(point);

		if (dist > radius)
		{
			point.x = (point.x * radius) / dist;
			point.y = (point.y * radius) / dist;
			point.z = (point.z * radius) / dist;
		}
	}
	else
	{
		rex_int height = PHYS_LENGTH(direction);

		rex_vec3i helper = PHYS_vec3Project(point,direction);
		rex_int y = PHYS_LENGTH(helper);

		helper = PHYS_vec3Minus(point,helper);

		rex_int x = PHYS_LENGTH(helper);

		if (x < 20)
		{
			// for such small distance big numeric errors occur in the other branch
			if (y >= height)
				point = direction;
		}
		else
		{
			rex_int scaledRadius = radius - ((y * radius) / height);

			if (y > height || x > scaledRadius) // outside?
			{
				if (x <= 0)
				{
					PHYS_LOG("WARNING: arithmetic error in envCone (library bug)");
					x = 1; // shouldn't happen but just in case, to prevent div by zero
				}

				helper.x = (helper.x * radius) / x;
				helper.y = (helper.y * radius) / x;
				helper.z = (helper.z * radius) / x;

				point = PHYS_envLineSegment(point,helper,direction); 
			}
		}
	}

	return PHYS_vec3Plus(point,center);
}

static inline rex_ubyte PHYS_bodyIsActive(const PHYS_body *body)
{
	return !(body->flags & PHYS_BODY_FLAG_DEACTIVATED);
}
