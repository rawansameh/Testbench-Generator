module led_sw(input [7:0] sw, output [7:0] led);
assign led[3] = sw[1];
endmodule