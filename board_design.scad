/**
 * @name AS5048 carrier board - v1.0
 * from: https://github.com/ZoetropeImaging/as5048A-breakout-board
 * @id as5080-board;
 * @category Misc
 */
module as5048_carrier() {
	difference(){
		union(){
			cube([45,35,1.6], center=true);
			//as5048
			translate([0,0,1.6/2]) color("grey") cube([4,4.5,1], center=true);
			//connector
			translate([9/2-45/2-0.5,0,-9/2-1.6/2]) color("grey") cube([9,18,9], center=true);
		}
		as5048_carrier_holes();
	}
}

/* Helper: Holes for AS5048 board */
module as5048_carrier_holes(){
	as5048_carrier_hole_positions() cylinder(h=1.8, r=1.5, center=true, $fn=20);
}

/* Helper: Hole posisitions for as5048 board*/
module as5048_carrier_hole_positions(){
	x_spacing = 39;
	y_spacing = 29;
	for(x = [-x_spacing/2, x_spacing/2])
		for(y = [-y_spacing/2, y_spacing/2])
			translate([x,y,0])
				children(0);
}

as5048_carrier();
