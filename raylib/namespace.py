import json
import os
import re

# Paths to the json file, src folder, and examples folder
json_path = 'parser/output/raylib_api.json'
src_folder = 'src'
examples_folder = 'examples'

# Load function and struct names from raylib_api.json
with open(json_path, 'r') as f:
    data = json.load(f)

# Assuming functions and structs are dictionaries with a "name" key
function_names = [func["name"] for func in data.get("functions", [])]
struct_names = [struct["name"] for struct in data.get("structs", [])]

# Exclude any already prefixed names or those that conflict in rlgl
excluded_names = {
    'LoadTexture', 'LoadTextureCubemap', 'UpdateTexture', 'UnloadTexture', 
    'GenTextureMipmaps', 'GetPixelDataSize'
}
function_names = [name for name in function_names if name not in excluded_names and not name.startswith('rl')]
struct_names = [name for name in struct_names if name not in excluded_names and not name.startswith('rl')]

# Combine and sort by length in descending order to handle overlapping names
all_names = function_names + struct_names
all_names.sort(key=len, reverse=True)

# Build a dictionary for replacement (e.g., "InitWindow" -> "rlInitWindow")
replace_dict = {name: f"rl{name}" for name in all_names}

# Function to replace text in a file
def replace_in_file(file_path, replace_dict):
    with open(file_path, 'r') as file:
        content = file.read()

    # Replace all occurrences of each name with its rl-prefixed version
    for original, replacement in replace_dict.items():
        content = re.sub(rf'\b{original}\b', replacement, content)

    # Write the modified content back to the file
    with open(file_path, 'w') as file:
        file.write(content)

# Function to traverse a directory and replace text in all source files
def replace_in_directory(folder_path, replace_dict):
    for root, _, files in os.walk(folder_path):
        for file_name in files:
            if file_name.endswith(('.c', '.h', '.cpp', '.hpp')):  # Adjust as needed for file types
                file_path = os.path.join(root, file_name)
                replace_in_file(file_path, replace_dict)

# Process both the src and examples folders
replace_in_directory(src_folder, replace_dict)
replace_in_directory(examples_folder, replace_dict)

print("Replacement complete!")
