# import toml
# from pprint import pprint

# # Your TOML content as a string
# toml_content = """
# [[faction]]
# name = "default"

# [[faction.general]]
# key = "cha_gen_guard_cav"
# max = "cavalry"
# script = "general_unit_"

# [[faction.levies]]
# key = "cha_gen_guard_cav"
# max = "cavalry"
# script = "general_unit_"

# [[faction.legion]]
# type = "Mande Hunter"
# key = "att_des_es_ashum"
# max = "ranged"
# script = "mande_hunter_unit_"

# [[faction.legion]]
# type = "Al-'Ashair Infantry"
# key = "cha_mus_berber_warriors"
# max = "infantry"
# script = "islamic_alashair_infantry_unit_"

# [faction.legion]
# type = "Sofa"
# key = "att_des_es_marz_archers"
# max = "ranged"
# script = "sofa_unit_"

# [[faction]]
# name = "arabic"

# [[faction.general]]
# key = "cha_gen_guard_cav"
# max = "cavalry"
# script = "general_unit_"

# [[faction.levies]]
# key = "cha_gen_guard_cav"
# max = "cavalry"
# script = "general_unit_"

# [[faction.legion]]
# type = "Mande Hunter"
# key = "att_des_es_ashum"
# max = "ranged"
# script = "mande_hunter_unit_"

# [[faction.legion]]
# type = "Al-'Ashair Infantry"
# key = "cha_mus_berber_warriors"
# max = "infantry"
# script = "islamic_alashair_infantry_unit_"

# [[faction.legion]]
# type = "Sofa"
# key = "att_des_es_marz_archers"
# max = "ranged"
# script = "sofa_unit_"
# """

# # Load the TOML content into a Python dictionary
# data = toml.loads(toml_content)

# # Pretty print the data structure
# pprint(data, sort_dicts=False)


file = "/home/dm/.local/share/Paradox Interactive/Imperator/logs/debug.log"

class UnitType:
	def __init__(self, name, count):
		self.name = name
		self.count = count

class UnitData:
	def __init__(self):
		self.attacker = False
		self.culture = ""
		self.commander = ""
		self.commander_culture = ""
		self.commander_martial = ""
		self.country_culture = ""
		self.units = list()

class DebugLogData:
	player = ""
	units = list()

	def __init__(self):
		with open(file, "r") as f:
			x = f.readlines()

		z = list()
		reading_combat_data = False
		unit_data = False

		for i in x:
			if "DEBUG_" not in i:
				continue
			i = i.split("DEBUG_")[1].strip()
			if i == "END":
				break
			if i == "START":
				reading_combat_data = True
				continue
			if not reading_combat_data:
				continue
			if "player:" in i:
				self.player = i.split("player: ")[1]
				continue
			if i == "NEWUNIT":
				if unit_data:
					self.units.append(unit_data)
				unit_data = UnitData()
				continue
			if "attacker:" in i:
				unit_data.attacker = True
				continue
			if "defender:" in i:
				unit_data.attacker = False
				continue
			if "country_culture:" in i:
				unit_data.culture = i.split("country_culture: ")[1]
				continue
			if "commander_culture:" in i:
				unit_data.commander_culture = i.split("commander_culture: ")[1]
				continue
			if "commander_martial:" in i:
				unit_data.commander_martial = i.split("commander_martial: ")[1]
				continue
			if "commander:" in i:
				unit_data.commander = i.split("commander: ")[1]
				continue

			# All other fields will be units formatted like "<unit_type>: 2.0"
			i = i.split(": ")
			name = i[0].replace(": ", "")
			count = i[1]
			if float(count) > 0:
				unit_data.units.append(UnitType(name, count))


		self.units.append(unit_data)

	def display(self):
		print(f"Player: {self.player}")
		print("----------------")
		for i in self.units:
			print(f"Attacker: {i.attacker}")
			print(f"Culture: {i.culture}")
			print(f"Commander: {i.commander}")
			print(f"Commander Culture: {i.commander_culture}")
			print(f"Commander Martial: {i.commander_martial}")
			#print(f"Units: {}")
			for j in i.units:
				print(f"{j.name}: {j.count}")
			print("----------------")


if __name__ == '__main__':
	x = DebugLogData()
	x.display()