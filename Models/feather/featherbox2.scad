
// Bottom
if (false)
difference(){
  cube([30, 58, 19]);
  translate([2, 2, -1])
    cube([26, 54, 18]);
  // USB hole
  for (i=[0:5]){
    translate([11-i/2, -1, 15-i])    
      cube([8+i, 40, 1]);
  }
  // Battery hole
  translate([20, 10, 8]){
      cube([15, 10, 7]);
  }  
  // APA102 hole
  translate([7, 56, 7]){
      cube([17, 20, 7]);
  }
}


if (false)
difference(){  
  union(){
    cube([30, 58, 2]);
    translate([15, 0, 0])
    cube([15, 29, 24]);
  }
  translate([12, 2, -1])
    cube([17, 25, 23]);
}

// For button feather wing
if (true)
difference(){  
  union(){
    cube([30, 58, 2]);
    cube([30, 26, 4]);
    translate([0,58-(20+5),0])
      cube([30, 26, 4]);
    // Left wing
    translate([3, 3, -6])
      cube([24, 1, 7]);
    // Right wing
    translate([3, 57, -6])
      cube([24, 1, 7]);
  }
  translate([15, 12, -1])
    cylinder(r1=8, r2=8, 8);
}