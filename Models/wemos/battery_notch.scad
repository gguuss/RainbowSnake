difference() {
  union(){
    cylinder(d1=32, d2=32, h=30);    
    translate([0,55,-5])
    rotate([90,270,0]){
      translate([5, -16, 0]){
        cube([30, 35, 110]);
      }
      translate([20, -16, 0]){
        cube([20, 35, 5]);
      }
      translate([20, -16, 105]){
        cube([20, 35, 5]);
      }      
    }
    translate([-55, 0, -5])
    rotate([180,270,0]){
      translate([5, -16, 0]){
        cube([30, 35, 110]);
      }
      translate([20, -16, 0]){
        cube([20, 35, 5]);
      }
      translate([20, -16, 105]){
        cube([20, 35, 5]);
      }      
    }
  }
  translate([0,0,-2])
  cylinder(d1=29, d2=29, h=25);
}