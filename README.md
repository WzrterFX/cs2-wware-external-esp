# cs2-wware-external-esp

## Description

External esp for the game Counter-Strike 2. This tool provides players with additional visual capabilities, allowing them to see the positions of opponents, their health, weapons, and other important parameters.

## Preview
https://youtu.be/57MyjJkQhz0

![image](https://github.com/WzrterFX/cs2-wware-external-esp/assets/122642787/4148fb35-5456-4ed3-b1eb-33dec45ba983)

![image](https://github.com/WzrterFX/cs2-wware-external-esp/assets/122642787/052646f4-d44e-4d36-b7b4-fb2b272fafe5)

![image](https://github.com/WzrterFX/cs2-wware-external-esp/assets/122642787/24d09131-7e77-4857-8059-24924ec292da)

## Features

### Entity

The `Entity` structure contains information about in-game entities, including position, health, team, and other parameters.

- `uintptr_t address`: The memory address of the entity.
- `int team`: The team of the entity.
- `int health`: The health of the entity.
- `Vector3 position`: The 3D position of the entity in the game world.
- `std::string name`: The name of the entity.
- `std::string weapon`: The current weapon of the entity.

- `uintptr_t address`: The memory address of the entity.
- `Vec3 position`: The 3D position of the entity in the game world.
- `Vec2 positionScreen`: The 2D screen position of the entity.
- `Vec3 height`: The 3D height of the entity in the game world.
- `Vec2 heightScreen`: The 2D screen height of the entity.
- `std::string name`: The name of the entity.
- `uint8_t team` The team of the entity.
-`uint8_t health`: The health of the entity.
- `bool spotted`: Whether the entity is spotted.
- `float distance`: The distance to the entity.
- `std::string weapon`: The current weapon of the entity.
- `std::vector<Vec3> bones`: The 3D positions of the entity's bones.
- `std::vector<Vec2> bonesScreen`: The 2D screen positions of the entity's bones.

### ESP Features

- **Box**: Draws boxes around players to show their position.
- **Bones**: Draws lines connecting the main bones of the player model for a more detailed position display.
- **Health**: Shows the health level of players.
- **Name**: Shows the names of players.
- **Weapon**: Shows the current weapon of players.
- **Team**: Displays only enemies or all players, depending on the settings.
- **Customizable Colors**: Allows customization of colors for different display elements.

### Memory

The `Memory` class provides interaction with the game process. Main functions:

- `CheckProcess(const wchar_t* name)`: Checks for the presence of a process with the given name.
- `ModifyProcess(const wchar_t* name)`: Modifies the process with the given name, opening it for reading/writing memory.
- `GetModule(const wchar_t* name)`: Returns the base address of the module with the given name.
- `ReadMemory(const uintptr_t& address)`: Reads data from memory at the specified address.
- `WriteMemory(const uintptr_t&, const T& value)`: Writes data to memory at the specified address.
- `ReadString(const uintptr_t& address, const size_t& size)`: Reads a string from memory at the specified address.
- `WriteString(const uintptr_t& address, const std::string& value)`: Writes a string to memory at the specified address.

### Renderer

The `Renderer` class handles the initialization and management of Direct3D rendering. Main functions:

- `Initialize()`: Initializes the Direct3D device.
- `ResetDevice()`: Resets the Direct3D device.
- `Cleanup()`: Cleans up the Direct3D device resources.
- `GetDevice() const`: Returns a pointer to the Direct3D device.
- `IsDeviceLost() const`: Checks if the device is lost.

### Application

The `Application` class manages the main application loop. Main functions:

- `Run()`: Runs the main application, initializing necessary components and managing the main loop.
- `Cleanup()`: Cleans up resources and terminates the application.

## How to Use

1. Download and extract the project.
2. Open the solution in your Visual Studio.
3. Build it.
4. Run the compiled executable.

## Dependencies

- DirectX SDK 2010
- ImGui

## Important

This project is intended for educational purposes only. Using esp in multiplayer games may violate game rules and result in an account ban. The developers are not responsible for any misuse of this software.
