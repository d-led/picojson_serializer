clang	:	
			clang++ test/const_strings_test.cpp test/custom_converter_example.cpp test/int64_test.cpp test/map_test.cpp test/non_class_test.cpp test/nonintrusive_test.cpp test/projections_test.cpp test/set_test.cpp test/test.cpp test/vector_test.cpp -I Catch/single_include -I picojson -o catch_test
			./catch_test
