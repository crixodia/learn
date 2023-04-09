from cmath import isnan
import pandas as pd
import pprint as pp

sheet_names = ["textures", "0", "1", "2", "3", "4", "5", "6"]
sheets = []

with pd.ExcelFile("layers.xlsx") as reader:
    for sn in sheet_names:
        sheets.append(pd.read_excel(reader, sheet_name=sn))

for i in range(1, len(sheets)):
    sheets[i].pop("Y")

texture_names = []
textures = {}
for s in sheets[0].values.tolist():
    textures[s[0]] = []
    texture_names.append(s[1])


def get_y(i):
    return 14 - i


def get_z(i):
    return -i - 1


for k, sheet in enumerate(sheets[1:]):
    aux = sheet.values.tolist()
    for i in range(len(aux)):
        for j in range(len(aux[0])):
            s = aux[i][j]
            if not isnan(s):
                textures[s].append(((k, get_y(i), get_z(j))))

# pp.pprint(textures)
texture_paths = f"const char * texture_paths[{len(texture_names)}] = {{"
position_data = []
dim_data = []
for i, path in enumerate(texture_names):
    texture_paths += "\n\t" + f'"textures/{path}",'
    path = path.replace(".png", "")
    position = f"glm::vec3 {path}[] = {{"
    dim_data.append(len(textures[i+1]))
    for pos in textures[i+1]:
        _s = str(tuple(map(float, pos))).replace(",", "f,").replace(")", "f)")
        position += "\n\t" + f'glm::vec3{_s},'
    position += "\n};"
    position_data.append(position)
texture_paths += "\n};"

with open('positions.txt', 'w') as f:
    f.write(
        "int dim[] = " +
        str(dim_data).replace('[', '{').replace(']', '};\n')
    )
    f.write(texture_paths+"\n")
    for _p in position_data:
        f.write(_p+"\n")
    f.close()
