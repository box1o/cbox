#include "cbox/graphics/shader/parser.hpp"
#include "cbox/core/core.hpp"
#include <algorithm>
#include <fstream>
#include <sstream>

namespace cc {

auto ShaderParser::Parse(const std::filesystem::path& filepath) -> result<void> {
    auto source_result = ReadFile(filepath);
    if (!source_result) {
        return err(source_result.error());
    }

    shader_name_ = filepath.stem().string();

    return ProcessSource(source_result.value());
}

auto ShaderParser::ParseString(std::string_view source) -> result<void> {
    return ProcessSource(source);
}

auto ShaderParser::ReadFile(const std::filesystem::path& filepath) -> result<std::string> {
    if (!std::filesystem::exists(filepath)) {
        return err(error_code::file_not_found, 
                   std::format("Shader file not found: {}", filepath.string()));
    }

    std::ifstream file(filepath, std::ios::in | std::ios::binary);
    if (!file.is_open()) {
        return err(error_code::file_access_denied,
                   std::format("Failed to open shader file: {}", filepath.string()));
    }

    std::string content;
    try {
        file.seekg(0, std::ios::end);
        const auto size = file.tellg();
        file.seekg(0, std::ios::beg);

        content.resize(static_cast<size_t>(size));
        file.read(content.data(), size);

        if (!file.good()) {
            return err(error_code::file_read_error,
                       std::format("Failed to read shader file: {}", filepath.string()));
        }
    } catch (const std::exception& e) {
        return err(error_code::file_read_error,
                   std::format("Exception reading shader file: {} - {}", 
                               filepath.string(), e.what()));
    }

    return ok(std::move(content));
}

auto ShaderParser::ProcessSource(std::string_view source) -> result<void> {
    if (source.empty()) {
        return err(error_code::validation_invalid_state, "Shader source is empty");
    }

    shader_sources_.clear();

    std::istringstream stream{std::string{source}};
    std::string line;
    ShaderStage current_stage = ShaderStage::Vertex;
    bool has_valid_stage = false;
    std::ostringstream current_source;

    while (std::getline(stream, line)) {
        const auto trimmed = [&line]() -> std::string_view {
            auto start = std::find_if_not(line.begin(), line.end(), 
                                          [](char c) { return std::isspace(c); });
            auto end = std::find_if_not(line.rbegin(), line.rend(),
                                        [](char c) { return std::isspace(c); }).base();
            return std::string_view(start, end);
        }();

        if (trimmed.starts_with(TYPE_TOKEN)) {
            if (has_valid_stage) {
                shader_sources_[current_stage] = current_source.str();
                current_source.str("");
                current_source.clear();
            }

            const auto type_name = trimmed.substr(TYPE_TOKEN.size());
            const auto type_trimmed = [&type_name]() -> std::string_view {
                auto start = std::find_if_not(type_name.begin(), type_name.end(),
                                              [](char c) { return std::isspace(c); });
                auto end = std::find_if_not(type_name.rbegin(), type_name.rend(),
                                            [](char c) { return std::isspace(c); }).base();
                return std::string_view(start, end);
            }();

            auto it = STAGE_MAP.find(type_trimmed);
            if (it == STAGE_MAP.end()) {
                return err(error_code::parse_invalid_format,
                           std::format("Unknown shader stage: {}", type_trimmed));
            }

            current_stage = it->second;
            has_valid_stage = true;

        } else if (trimmed.starts_with(NAME_TOKEN)) {
            const auto name = trimmed.substr(NAME_TOKEN.size());
            const auto name_trimmed = [&name]() -> std::string_view {
                auto start = std::find_if_not(name.begin(), name.end(),
                                              [](char c) { return std::isspace(c); });
                auto end = std::find_if_not(name.rbegin(), name.rend(),
                                            [](char c) { return std::isspace(c); }).base();
                return std::string_view(start, end);
            }();

            shader_name_ = name_trimmed;

        } else if (has_valid_stage) {
            current_source << line << '\n';
        }
    }

    if (has_valid_stage) {
        shader_sources_[current_stage] = current_source.str();
    }

    if (shader_sources_.empty()) {
        return err(error_code::parse_invalid_format, "No shader stages found in source");
    }

    log::Info("Parsed shader '{}' with {} stage(s)", shader_name_, shader_sources_.size());

    return ok();
}

}
