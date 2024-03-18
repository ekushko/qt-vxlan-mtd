#pragma once

#define CB_FILL_MAP(Combo, Map) for (auto it = Map.begin(); it != Map.end(); ++it) Combo->addItem(it.value(), it.key())
#define CB_DATA_TOE(Combo, Enum) static_cast<Enum>(Combo->currentData().toInt())
#define CB_DATA_TOI(Combo) Combo->currentData().toInt()
#define CB_SELECT(Combo, Value) Combo->setCurrentIndex(Combo->findData(static_cast<int>(Value)))
#define CB_SET_INDEX(Combo, Text) Combo->setCurrentIndex(Combo->findText(Text))
