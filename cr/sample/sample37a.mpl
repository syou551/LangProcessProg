program sample33p; {reference array from procedure and def array as parameter}
var a,x : integer;
var b : char;
var point : array[100] of integer;
procedure q(b:integer);
  var a : boolean;
      q : array[100] of integer;
begin
	writeln('proc of q');
    q[99] := x;
    point[99] := q[99];
	a := b = q[100];
    point[0] := q[0];
	if a then writeln('true') else writeln('false')
end;
var c : integer;
begin
	a := 1;		b := 'b';
	call q(a)
end.