#include "build.hh"
#include <cstdio>
#include <filesystem>
#include <functional>
#include <ranges>
#include <unordered_map>
#include <vector>

namespace fs = std::filesystem;
namespace ranges = std::ranges;
namespace views = std::ranges::views;
using namespace Kitchen;

void help()
{
	std::cout << "[HELP]: provide one of the arguments:" << std::endl;
	std::cout << "[HELP]: -> run - compile and run the app" << std::endl;
	std::cout << "[HELP]: -> help - display help" << std::endl;
	std::cout << "[HELP]: -> build - compile the app" << std::endl;
	std::cout << "[HELP]: -> clean - remove build artefacts" << std::endl;
}

int main(int argc, char** argv)
{
	Chef chef{};

	Sink::stage(2);

	{
		const char* output = "external/raylib-5.0_linux_amd64/";
		CommandRecipe download_raylib{};
		CommandRecipe unpack_raylib{};

		auto comp = [](const std::string& output) -> bool { return !fs::exists(output); };

		download_raylib.output(output).cache().cache_func(comp).push(
			{"wget", "-P", "./external", "https://github.com/raysan5/raylib/releases/download/5.0/raylib-5.0_linux_amd64.tar.gz"});

		unpack_raylib.output(output).cache().cache_func(comp).push(
			{"tar", "xzf", "./external/raylib-5.0_linux_amd64.tar.gz", "-C", "./external"});

		chef.cook(&download_raylib);
		chef.cook(&unpack_raylib);
	}

	auto non_main_file_names = ranges::subrange(fs::directory_iterator("."), fs::directory_iterator())
							 | views::filter([](auto&& value) {
								   fs::path path = value;
								   return path.has_extension() && path.extension() == ".cpp";
							   })
							 | views::filter([](auto&& value) {
								   fs::path path = value;
								   return (!(path.filename() == "nob.cpp" || path.filename() == "nob_stage2.cpp"
											 || path.filename() == "main.cpp"));
							   })
							 | views::transform([](auto&& value) {
								   auto fullname = ((fs::path)value).string();
								   size_t lastindex = fullname.find_last_of(".");
								   std::string name = fullname.substr(0, lastindex);
								   return name.substr(2, name.size());
							   })
							 | std::views::common | ranges::to<std::vector<std::string>>();

	LineCook line_cook{};
	Ingredients o_files{};
	Ingredients created_files{};
	CppRecipe main("main recipe");

	(void)Sink::shift_args(&argc, &argv); // discard executable name

	Ingredients flags{};
	flags += "-c";

	if (non_main_file_names.size() > 0) {
		for (const auto& filename : non_main_file_names) {
			auto files = new Ingredients();
			auto o_file = "build/" + filename + ".o";
			created_files.add_ingredients(o_file);
			files->add_ingredients(filename + ".cpp");

			auto recipe = new CppRecipe("");
			recipe->compiler(CC)
				.push(flags.get_ingredients())
				.cache()
				.output(o_file)
				.cpp_version("c++23")
				.files(*files);

			line_cook += recipe;
			o_files.add_ingredients(o_file);
		}

		int status = line_cook.cook();
		if (status != 0) std::exit(status);
	}

	o_files += "main.cpp";
	main.compiler(CC)
		.cpp_version("c++23")
		.push({"-I./external/raylib-5.0_linux_amd64/include/"})
		.files(o_files)
		.push({"-L./external/raylib-5.0_linux_amd64/lib/"})
		.push({"-l:libraylib.a", "-lm", "-pthread"})
		.output("bin/main");

	created_files.add_ingredients("bin/main");

	std::unordered_map<std::string, std::function<void()>> arg_handler{};
	arg_handler.insert_or_assign("run", [&]() {
		chef.cook(&main);
		std::cout << std::endl;
		Sink::start_job_sync({"./bin/main"});
	});
	arg_handler.insert_or_assign("build", [&]() { chef.cook(&main); });
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
