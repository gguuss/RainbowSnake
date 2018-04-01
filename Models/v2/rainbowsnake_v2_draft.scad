difference () {
  cube([80, 40, 3]);
  
  translate([5, 17, -1])    
    cube([7, 7, 7]);
      
  translate([10, 0, 0]){
    // X-Y POT dials
    translate([10, 7, -5])
      cylinder(h=15,d=10);
    translate([10, 33, -5])
      cylinder(h=15,d=10);
    
    // RGB Rotary Encoder + Button
    translate([30, 20, -5])
      cylinder(h=15,d=10);
    
    // Button row
    // Little buttons
    // 
    //translate([50, 10, -5])
    //  cylinder(h=15,d=5);
    //translate([50, 20, -5])
    //  cylinder(h=15,d=5);    
    //translate([50, 30, -5])
    //  cylinder(h=15,d=5);
    
    // Button row
    // Arcade button (duh)
    translate([53, 20, -5])
      cylinder(h=15,d=24);
  }  
}
/*
// Auto / Man text
rotate([0, 0, 90])
translate([20, -2, 2])
linear_extrude(height = 2) {
text("Auto / Btn", 4, "Liberation Sans",
    halign = "center", valign = "center");
}

// Mode text
rotate([0, 0, 90])
translate([20, -30, 2])
linear_extrude(height = 2) {
  text("Mode", 4, "Liberation Sans",
      halign = "center", valign = "center");
}

// Dimmer
translate([42, 37, 2])
linear_extrude(height = 2) {
  text("Dimmer", 4, "Liberation Sans",
      halign = "center", valign = "center");
}
*/