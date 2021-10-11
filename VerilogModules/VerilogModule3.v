module mux41 (input [3:0] a, [3:0] b, [3:0] c, [3:0] d, [1:0] sel, output [3:0] out);
assign out = s1 ? (s0 ? d : c) : (s0 ? b : a);
endmodule