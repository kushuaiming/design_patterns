project(behavioral_patterns)

add_executable(chain_of_responsibility chain_of_responsibility.cc)
add_executable(command command.cc)
add_executable(iterator iterator.cc)
add_executable(mediator mediator.cc)
add_executable(memento memento.cc)
add_executable(observer observer.cc)
add_executable(state state.cc)
add_executable(strategy strategy.cc)
add_executable(template template.cc)
add_executable(visitor visitor.cc)

target_link_libraries(state
    sub::common
)