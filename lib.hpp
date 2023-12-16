#include <gperftools/profiler.h>

#include <string>
#include <vector>
#include <memory>
#include <ranges>


class Document {
public:
    std::string text;
};

class EditorCommand {
public:
    virtual ~EditorCommand() = default;

    virtual void rollback(Document& document) const = 0;
    virtual void apply(Document& document) const = 0;
};

class TextEditCommand: public EditorCommand {
public:
    TextEditCommand(std::string new_text, int position);

    virtual void rollback(Document& document) const override {
        document.text.erase(position, new_text.size());
    }

    virtual void apply(Document& document) const override {
        document.text.insert(position, new_text);
    }

private:
    std::string new_text;
    int position;
};


class MacroCommand: public EditorCommand {
public:

    void add_command(std::shared_ptr<EditorCommand> cmd) {
        commands.emplace_back(std::move(cmd));
    }

    virtual void rollback(Document& document) const override {
        for (auto& command: commands) {
            command->rollback(document);
        }
    }

    virtual void apply(Document& document) const override {
        for (auto& command: commands) {
            command->apply(document);
        }
    }

private:
    std::vector<std::shared_ptr<EditorCommand>> commands;
};

class Editor {
public:

    void on_text_input(int position, std::string text) {
        auto cmd = std::make_unique<TextEditCommand>(text, position);
        if (is_recording_macro) {
            recorded_macro_command->add_command(std::move(cmd));
        } else {
            cmd->apply(document);
            command_history.emplace_back(std::move(cmd));
        }
    }

    void on_undo_pressed() {
        if (command_history.empty()) return;
        auto cmd = std::move(command_history.back());
        cmd->rollback(document);
        command_history.pop_back();
        undone_command_history.push_back(std::move(cmd));
    }

    void on_redo_pressed() {
        if (undone_command_history.empty()) return;
        auto cmd = std::move(undone_command_history.back());
        cmd->apply(document);
        undone_command_history.pop_back();
        command_history.push_back(std::move(cmd));
    }

    void on_record_macro_start() {
        is_recording_macro = true;
        recorded_macro_command = std::make_unique<MacroCommand>();
    }

    void on_record_macro_end() {
        is_recording_macro = false;
        macros.push_back(recorded_macro_command);
    }

private:
    Document document;
    std::vector<std::shared_ptr<EditorCommand>> command_history;
    std::vector<std::shared_ptr<EditorCommand>> undone_command_history;

    bool is_recording_macro = false;
    std::shared_ptr<MacroCommand> recorded_macro_command;

    std::vector<std::shared_ptr<MacroCommand>> macros;
};
