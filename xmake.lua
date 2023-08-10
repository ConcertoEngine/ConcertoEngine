add_rules('mode.debug')
add_repositories('Concerto-xrepo https://github.com/ConcertoEngine/xmake-repo.git main')

add_requires('gtest', 'ConcertoCore', 'ConcertoGraphics')
add_requireconfs("**.vulkan-headers", {override = true, version = "1.3.246"})

target('Concerto')
    set_kind('static')
    set_symbols('debug')
    set_warnings('allextra')
    set_languages('cxx20')
    add_includedirs('Include', 'Include/Ecs', 'Include/Ecs/Systems', 'Include/Ecs/Components')
    add_files('Src/**.cpp')
    add_packages('ConcertoCore', 'ConcertoGraphics')

target('ConcertoUnitTests')
    set_kind('binary')
    set_symbols('debug')
    set_warnings('everything')
    set_languages('cxx20')
    set_optimize('none')
    add_includedirs('Include', 'Include/Ecs', 'Include/Ecs/Systems')
    add_files('tests/*.cpp')
    add_packages('gtest', 'ConcertoCore')
    add_deps('Concerto')
