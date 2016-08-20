bDrawHat = true;
bDrawOuterCylinder = false;
bDrawInnerCylinder = false;

// Inner cylinder
innerHeight=50;
innerCylinderOuter = 25;
innerCylinderInner = 23;
innerCylinderBevel = 5;


// Outer cylinder
outerCylinderHole = innerCylinderOuter+innerCylinderBevel+1;
outerCylinderOuter = outerCylinderHole + 2;


// Hat
hatInnerCylinderSize = outerCylinderOuter + 1;
hatOuterCylinderSize = hatInnerCylinderSize + 4; // beefy
hatHeight = innerHeight / 2 + 10;
topSize = innerCylinderInner / 2;


if (bDrawInnerCylinder) {
    drawInnerCylinder();
}

if (bDrawOuterCylinder) {
    translate([0,0,-70])
        drawOuterCylinder();
}

if (bDrawHat){
    translate([0,0,70])
        drawHat();
}


module drawInnerCylinder() {
    difference() {
        union() {
            cylinder(r1=innerCylinderOuter+innerCylinderBevel, 
                r2=innerCylinderOuter + innerCylinderBevel, h=innerHeight / 10);
            cylinder(r1=innerCylinderOuter, r2=innerCylinderOuter, h=innerHeight);
            translate([0,0,innerHeight - (innerHeight / 10)])
                cylinder(r1=innerCylinderOuter+innerCylinderBevel, 
                    r2=innerCylinderOuter+innerCylinderBevel, h=innerHeight / 10);
        }
        translate([0,0,2])
            cylinder(r1=innerCylinderInner, r2=innerCylinderInner, h=innerHeight);        
    }
}

module drawOuterCylinder() {
    difference() {
        union() {
            cylinder(r1=outerCylinderOuter, r2=outerCylinderOuter, h=innerHeight + 5);
            // TODO: screw part?
            rotate([90,0,0])
                translate([0,53,-38])
                cylinder(r1=3, r2=3, innerCylinderOuter * 3);
        }
        translate([0,0,2])
            cylinder(r1=outerCylinderHole, r2=outerCylinderHole, h=innerHeight + 5);
    }    
}

module drawHat() {    
    // section that screws in
    difference() {
        union() {
            cylinder(r1=hatOuterCylinderSize, r2=hatOuterCylinderSize, h=hatHeight);
            translate([0,0,hatHeight])
                cylinder(r1=hatOuterCylinderSize, r2=topSize, h=hatHeight);

    
        }
        // Hollow inner
        translate([0,0,-2])
            cylinder(r1=hatInnerCylinderSize, r2=hatInnerCylinderSize, h=hatHeight);       
        
        // Hollow top
        translate([0,0,hatHeight - 3])
            cylinder(r1=hatInnerCylinderSize, r2=topSize - 5, h=hatHeight - 5);

        
        translate([0,50,hatHeight + 25])
            rotate([90, 0, 0])
                cylinder(r1=topSize * 2 / 3, r2=topSize * 2 / 3, h=150);
        
        // Side A slider
        translate([-3, 30, -2])
          cube([6, 10, 20]);        
        translate([-17, 30, 22])
            rotate([0, 90,0])
                cube([6, 10, 20]);
        
        
        // Side B slider
        translate([-3, -38, -2])
          cube([6, 10, 20]);
        
        translate([-3, -38, 22])
            rotate([0, 90,0])
                cube([6, 10, 20]);    
    }
        
}