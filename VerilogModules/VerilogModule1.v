module FullAdder(input A, B, Cin, output sum, Cout);
assign {Cout, sum} = A + B + Cin;
endmodule