#include "build.hh"

int main(int argc, char** argv)
{
	GO_REBUILD_YOURSELF(argc, argv);
	Kitchen::Sink::stage(1);

	std::filesystem::path executable_path = Kitchen::Sink::shift_args(&argc, &argv);
	std::string executable_str = executable_path.filename().string();
	std::string exec_stage2 = "bin/" + executable_str + "_stage2";

	auto files = Kitchen::Ingredients().add_ingredients("nob_stage2.cpp");
	auto recipe = Kitchen::CompilerRecipe("nob_stage2")
					  .compiler(CC)
					  .std_version("c++23")
					  .optimization(Kitchen::Heat::Oz)
					  .files(files)
					  .output(exec_stage2)
					  .cache();

	Kitchen::Chef chef{};

	int status = chef.cook(&recipe);

	if (status == 0) {
		std::vector<std::string> command(argv, argv + argc);
		command.insert(command.begin(), exec_stage2);
		Kitchen::Sink::start_job_sync(command);
	} else {
		Kitchen::Sink::log(Kitchen::Sink::LogLevel::ERROR, "failed to compile stage 2");
	}
}
