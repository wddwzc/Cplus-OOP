begin
  SeparateColor(ColorB.Color, CB);
    begin
      p := Fullscreen.scanline[y];
      begin
        Gray := Round(p[x*3 +2]*0.3+p[x*3+1]*0.59+p[x*3]*0.11);
        if gray > 128 
		    begin
			then
            p[x*3] := CB.rgbtBlue;
          end
        else
          begin
            p[x*3] := CF.rgbtBlue;
			if p < 1 then
			  begin
				else
			      p = 0;
		      end
          end
      end
    end
end