#pragma once

namespace game::commands {

    struct CreateMap;
    struct Spawn;
    struct March;
    struct Wait;
    struct Finish;
    /// @todo add_command

    struct Visitor
    {
        virtual void Process(const CreateMap& command) = 0;
        virtual void Process(const Spawn& command) = 0;
        virtual void Process(const March& command) = 0;
        virtual void Process(const Wait& command) = 0;
        virtual void Process(const Finish& command) = 0;
        /// @todo add_command
    };

}
