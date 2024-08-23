#include "../build.hh"
#include <cstdio>
#include <filesystem>
#include <functional>
#include <ranges>
#include <unordered_map>
#include <vector>

using namespace Kitchen;

namespace fs = std::filesystem;
namespace views = std::ranges::views;
namespace ranges = std::ranges;

void help()
{
	std::cout << std::endl;
	std::cout << "[HELP] provide one of the arguments:" << std::endl;
	std::cout << "[HELP]	-> run - compile and run the app" << std::endl;
	std::cout << "[HELP]	-> help - display help" << std::endl;
	std::cout << "[HELP]	-> build - compile the app" << std::endl;
	std::cout << "[HELP]	-> clean - remove build artefacts" << std::endl;
}

int main(int argc, char** argv)
{
	const char* raylib = "external/raylib-5.0_linux_amd64/";
	Sink::stage(2);

	if (!fs::exists(raylib)) {
		Sink::start_job_sync({"wget", "-P", "./external",
							  "https://github.com/raysan5/raylib/releases/download/5.0/raylib-5.0_linux_amd64.tar.gz"});
		Sink::start_job_sync({"tar", "xzf", "./external/raylib-5.0_linux_amd64.tar.gz", "-C", "./external"});
	}

	auto non_main_file_names = ranges::subrange(fs::directory_iterator("./src"), fs::directory_iterator())
							 | views::filter([](auto&& value) {
								   fs::path path = value;
								   return path.has_extension() && path.extension() == ".cc";
							   })
							 | views::filter([](auto&& value) {
								   fs::path path = value;
								   return (!(path.filename() == "nob.cc" || path.filename() == "nob_stage2.cc"
											 || path.filename() == "main.cc"));
							   })
							 | views::transform([](auto&& value) {
								   auto fullname = ((fs::path)value).filename().string();
								   size_t lastindex = fullname.find_last_of(".");
								   return fullname.substr(0, lastindex);
							   })
							 | std::views::common | ranges::to<std::vector<std::string>>();

	LineCook line_cook{};
	Ingredients o_files{};
	Ingredients created_files{};
	CompilerRecipe main("main recipe");

	(void)Sink::shift_args(&argc, &argv); // discard executable name

	Ingredients flags{};
	flags += "-c";
	flags += "-Wall";
	flags += "-pedantic";

	if (non_main_file_names.size() > 0) {
		for (const auto& filename : non_main_file_names) {
			auto files = new Ingredients();
			auto o_file = "build/" + filename + ".o";
			created_files.add_ingredients(o_file);
			files->add_ingredients("src/" + filename + ".cc");

			auto recipe = new CompilerRecipe("");
			recipe->compiler(CC)
				.cache()
				.std_version("c++23")
				.push(flags.get_ingredients())
				.output(o_file)
				.files(*files);

			line_cook += recipe;
			o_files.add_ingredients(o_file);
		}

		int status = line_cook.cook();
		if (status != 0) {
			Sink::log(Sink::LogLevel::ERROR, "failed to build .o files");
			std::exit(status);
		}
	}

	o_files += "src/main.cc";
	main.compiler(CC)
		.cache()
		.std_version("c++23")
		.push({"-I./external/raylib-5.0_linux_amd64/include/"})
		.files(o_files)
		.push({"-L./external/raylib-5.0_linux_amd64/lib/"})
		.push({"-l:libraylib.a", "-lm", "-pthread"})
		.push({"-Wall", "-pedantic", "-Werror"})
		.output("bin/main");

	created_files.add_ingredients("bin/main");

	std::unordered_map<std::string, std::function<void()>> arg_handler{};
	arg_handler.insert_or_assign("run", [&]() {
		if (Kitchen::cook(&main) != 0)
			std::exit(1);
		std::cout << std::endl;
		Sink::start_job_sync({"./bin/main"});
	});
	arg_handler.insert_or_assign("build", [&]() { std::exit(Kitchen::cook(&main)); });
	arg_handler.insert_or_assign("clean", [&]() {
		for (const auto& file : created_files.get_ingredients()) {
			Sink::print_command({"rm", "-rf", file});
			std::remove(file.c_str());
		}
	});
	arg_handler.insert_or_assign("help", []() { help(); });

	if (argc < 0) {
		help();
		return 1;
	}

	const char* arg;
	if ((arg = Sink::shift_args(&argc, &argv)) != nullptr) {
		if (arg_handler.contains(arg))
			arg_handler[arg]();
		else {
			Sink::log(Sink::LogLevel::ERROR, std::format("unknown argument: {}", arg));
			help();
		}
	} else
		arg_handler["build"]();
}
