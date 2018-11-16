ps03-6 : ps03-6.cpp
	g++ -std=gnu++0x -o $@ $<
	./$@

ps03-4 : ps03-4.cpp
	g++ -std=gnu++0x -o $@ $<
	./$@ > ps03-4-real.txt
	diff -y ps03-4-mind.txt ps03-4-real.txt

run-% : run-%.cpp %.cpp
	g++ -O2 -Wall -Werror -Wformat-security -Wignored-qualifiers -Winit-self -Wswitch-default -Wfloat-equal -Wshadow -Wpointer-arith -Wtype-limits -Wempty-body -Wlogical-op -Wmissing-field-initializers -Wctor-dtor-privacy  -Wnon-virtual-dtor -Wstrict-null-sentinel  -Wold-style-cast -Woverloaded-virtual -Wsign-promo -Weffc++ -std=gnu++0x -o $@ $<
	./$@

% : %.cpp
	g++ -O2 -Wall -Werror -Wformat-security -Wignored-qualifiers -Winit-self -Wswitch-default -Wfloat-equal -Wshadow -Wpointer-arith -Wtype-limits -Wempty-body -Wlogical-op -Wmissing-field-initializers -Wctor-dtor-privacy  -Wnon-virtual-dtor -Wstrict-null-sentinel  -Wold-style-cast -Woverloaded-virtual -Wsign-promo -Weffc++ -std=gnu++0x -o $@ $<
	./$@
#	g++ -O2 -Wall -Werror -Wformat-security -Wignored-qualifiers -Winit-self -Wswitch-default -Wfloat-equal -Wshadow -Wpointer-arith -Wtype-limits -Wempty-body -Wlogical-op -Wmissing-field-initializers -Wctor-dtor-privacy  -Wnon-virtual-dtor -Wstrict-null-sentinel  -Wold-style-cast -Woverloaded-virtual -Wsign-promo -Weffc++ -std=gnu++0x -o $@ $<
#	./$@
