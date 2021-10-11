module counter(input clk, rst, output [3:0] out);
always @ (posedge clk) begin
	if (! rst)
		out <=0;
	else
		out <= out + 1;
	end
endmodule