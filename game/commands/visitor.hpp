#pragma once

namespace game::commands {

    struct CreateMap;
    struct Spawn;
    /// @todo add_command

    struct Visitor
    {
        virtual void Process(const CreateMap& command) = 0;
        virtual void Process(const Spawn& command) = 0;
        /// @todo add_command
    };

}
