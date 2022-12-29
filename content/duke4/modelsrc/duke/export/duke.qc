// model info
$modelname "duke_nukem_endangered_species/duke.mdl"
$surfaceprop "armorflesh"
$cdmaterials "models/duke_nukem_endangered_species"

// base mesh
$body body "Duke Body.smd"

// shotgun bodygroup
$bodygroup "shotgun"
{
	blank
	studio "Duke Shotgun.smd"
}

// equipment bodygroup
$bodygroup "equipment"
{
	blank
	studio "Duke Equipment.smd"
}

// ragdoll
$collisionjoints "Duke Ragdoll.smd"
{
	$mass 90.0
	$inertia 10.00
	$damping 0.01
	$rotdamping 1.50

	// root bone
	$rootbone "pelvis"

	// body

	$jointconstrain "torso" x limit -48.00 48.00 1.00
	$jointconstrain "torso" y limit -25.00 25.00 1.00
	$jointconstrain "torso" z limit -25.00 50.00 1.00

	$jointconstrain "head" x limit -20.00 20.00 1.00
	$jointconstrain "head" y limit -25.00 25.00 1.00
	$jointconstrain "head" z limit -13.00 30.00 1.00

	// right arm

	$jointconstrain "rarmup" x limit -39.00 39.00 1.00
	$jointconstrain "rarmup" y limit -79.00 95.00 1.00
	$jointconstrain "rarmup" z limit -93.00 23.00 1.00

	$jointconstrain "rarmlow" x limit 0.00 0.00 1.00
	$jointconstrain "rarmlow" y limit 0.00 0.00 1.00
	$jointconstrain "rarmlow" z limit -149.00 4.00 1.00

	$jointconstrain "rhand" x limit -60.00 60.00 1.00
	$jointconstrain "rhand" y limit 0.00 0.00 0.00
	$jointconstrain "rhand" z limit -57.00 70.00 1.00

	// left arm

	$jointconstrain "larmup" x limit -30.00 30.00 1.00
	$jointconstrain "larmup" y limit -95.00 84.00 1.00
	$jointconstrain "larmup" z limit -86.00 26.00 1.00

	$jointconstrain "larmlow" x limit 0.00 0.00 1.00
	$jointconstrain "larmlow" y limit 0.00 0.00 1.00
	$jointconstrain "larmlow" z limit -149.00 4.00 1.00

	$jointconstrain "lhand" x limit -37.00 37.00 1.00
	$jointconstrain "lhand" y limit 0.00 0.00 1.00
	$jointconstrain "lhand" z limit -57.00 59.00 1.00

	// right leg

	$jointconstrain "rlegup" x limit -12.00 12.00 1.00
	$jointconstrain "rlegup" y limit -8.00 75.00 1.00
	$jointconstrain "rlegup" z limit -97.00 32.00 1.00

	$jointconstrain "rleglow" x limit 0.00 0.00 1.00
	$jointconstrain "rleglow" y limit 0.00 0.00 1.00
	$jointconstrain "rleglow" z limit -12.00 126.00 1.00

	$jointconstrain "rfoot" x limit 0.00 0.00 0.00
	$jointconstrain "rfoot" y limit -25.00 6.00 1.00
	$jointconstrain "rfoot" z limit -15.00 35.00 1.00

	// left leg

	$jointconstrain "llegup" x limit -12.00 12.00 1.00
	$jointconstrain "llegup" y limit -73.00 6.00 1.00
	$jointconstrain "llegup" z limit -93.00 30.00 1.00

	$jointconstrain "lleglow" x limit 0.00 0.00 1.00
	$jointconstrain "lleglow" y limit 0.00 0.00 1.00
	$jointconstrain "lleglow" z limit -8.00 126.00 1.00

	$jointconstrain "lfoot" x limit 0.00 0.00 0.00
	$jointconstrain "lfoot" y limit -19.00 19.00 1.00
	$jointconstrain "lfoot" z limit -15.00 35.00 1.00
}

$sequence ragdoll "Duke Ragdoll.smd" fps 30 activity ACT_DIERAGDOLL 1
