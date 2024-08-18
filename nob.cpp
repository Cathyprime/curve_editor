#include "build.hh"

int main(int argc, char** argv)
{
	GO_REBUILD_YOURSELF(argc, argv);
	KitchenSink::stage(1);

	std::filesystem::path executable_path = KitchenSink::shift_args(&argc, &argv);
	std::string executable_str = executable_path.filename().string();
	std::string exec_stage2 = "bin/" + executable_str + "_stage2";

	auto files = Kitchen::Ingredients().add_ingredients("nob_stage2.cpp");
	auto recipe = Kitchen::CppRecipe("nob_stage2")
					  .compiler(CC)
					  .cpp_version("c++23")
					  .optimization(Kitchen::Heat::Oz)
					  .files(files)
					  .output(exec_stage2)
					  .cache();

	Kitchen::Chef chef{};

	chef += &recipe;
	int status = chef.cook();

	if (status == 0) {
		std::vector<std::string> command(argv, argv + argc);
		command.insert(command.begin(), exec_stage2);
		KitchenSink::start_job_sync(command);
	} else {
		KitchenSink::log(KitchenSink::LogLevel::ERROR, "failed to compile stage 2");
	}
}
