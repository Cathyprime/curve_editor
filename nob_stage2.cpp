#include "build.hh"
#include <cstdio>
#include <filesystem>
#include <functional>
#include <ranges>
#include <unordered_map>
#include <vector>

namespace fs = std::filesystem;
namespace ranges = std::ranges;

void help()
{
	std::cout << "provide one of the arguments:" << std::endl;
	std::cout << " - run" << std::endl;
	std::cout << " - build" << std::endl;
	std::cout << " - clean" << std::endl;
}

int main(int argc, char** argv)
{
	KitchenSink::stage(2);

	auto non_main_file_names = ranges::subrange(fs::directory_iterator("."), fs::directory_iterator())
							 | ranges::views::filter([&](auto&& value) {
								   fs::path path = value;
								   return path.has_extension() && path.extension() == ".cpp";
							   })
							 | ranges::views::filter([&](auto&& value) {
								   fs::path path = value;
								   return (!(path.filename() == "nob.cpp" || path.filename() == "nob_stage2.cpp"
											 || path.filename() == "main.cpp"));
							   })
							 | ranges::views::transform([&](auto&& value) {
								   auto fullname = static_cast<fs::path>(value).string();
								   size_t lastindex = fullname.find_last_of(".");
								   std::string rawname = fullname.substr(0, lastindex);
								   return rawname.substr(2, rawname.size());
							   })
							 | std::views::common | ranges::to<std::vector<std::string>>();

	Kitchen::LineCook line_cook{};
	Kitchen::Chef chef{};
	Kitchen::Ingredients o_files{};
	Kitchen::Ingredients created_files{};
	Kitchen::CppRecipe main("main recipe");

	(void)KitchenSink::shift_args(&argc, &argv); // discard executable name

	Kitchen::Ingredients flags{};
	flags += "-c";

	Kitchen::Ingredients libs{};
	libs += "raylib";

	chef += &main;

	if (non_main_file_names.size() > 0) {
		for (const auto& filename : non_main_file_names) {
			auto files = new Kitchen::Ingredients();
			auto o_file = "build/" + filename + ".o";
			created_files.add_ingredients(o_file);
			files->add_ingredients(filename + ".cpp");

			auto recipe = new Kitchen::CppRecipe("");
			recipe->compiler(CC).cflags(flags).cache().output(o_file).cpp_version("c++23").files(*files);

			line_cook += recipe;
			o_files.add_ingredients(o_file);
		}

		int status = line_cook.cook();
		if (status != 0) std::exit(status);
	}

	o_files += "main.cpp";
	main.compiler(CC).output("bin/main").files(o_files).cpp_version("c++23").cache().libraries(libs);
	created_files.add_ingredients("bin/main");

	std::unordered_map<std::string, std::function<void()>> arg_handler{};
	arg_handler.insert_or_assign("run", [&]() {
		chef.cook();
		std::cout << std::endl;
		KitchenSink::start_job_sync({"./bin/main"});
	});
	arg_handler.insert_or_assign("build", [&]() { chef.cook(); });
	arg_handler.insert_or_assign("clean", [&]() {
		for (const auto& file : created_files.get_ingredients()) {
			KitchenSink::print_command({"rm", "-rf", file});
			std::remove(file.c_str());
		}
	});

	if (argc < 0) {
		help();
		return 1;
	}

	const char* arg;
	if ((arg = KitchenSink::shift_args(&argc, &argv)) != nullptr) {
		if (arg_handler.contains(arg))
			arg_handler[arg]();
		else
			KitchenSink::log(KitchenSink::LogLevel::ERROR, std::format("unknown argument: {}", arg));
	} else
		arg_handler["build"]();
}
