# Ticket: one naming convention for the Lib classes and functions

Status: open

## Problem

The Lib names do not follow one rule:

- Shims of Qt classes have the Qt name with an A in front: `ALabel`, `APushButton`, `ACheckBox` and the others, in files
  such as `alabel.h`. These were added so that one change to a control applies everywhere.
- Some Lib classes keep another prefix or no prefix: `AccessibleDateTimeEdit`, `AccessibleDateRangeEdit`,
  `AccessibleStringListModel`, `WarningYesNo`, `WarningOk`, `WarningChoice`, `Search`, `ItemSelect`,
  `Encryption`. The Safe prefix was replaced by the A prefix (`SafeLineEdit` is now `ALineEdit`, and its file `alineedit.h`),
  in the Lib and in ContactManager, EasyNotepad and ParseWeb.
- Functions are named by what they do, not by the Qt class they replace: `inputDialog`, `warningOkDialog`,
  `deliberateYesNoDialog`, `criticalDialog`, `execFocused`.

A reader cannot tell from a name which Qt class a Lib class replaces, or which file holds it.

## Proposal

1. Decide the rule: a Lib class that is a shim of a Qt class QWibble is named AWibble, and its file is awibble.h. A Lib
   class with no Qt counterpart keeps its name, and its file has the same name in lower case.
2. Decide how a Lib function that replaces a Qt function is named (for example `AFileDialog::getOpenFileName` is the shim
   of `QFileDialog::getOpenFileName`). Functions that are not shims keep their names.
3. List every Lib name that does not follow the rule, and a new name for each.
4. Rename in steps. Keep the old name as an inline alias for one release, so that EasyNotepad, ParseWeb and any other
   program that uses Lib still builds.
5. Update the .pro files, the .ui files that promote the classes, and the gotchas.

## Acceptance

- Every Lib class, file and shim function follows the rule.
- ContactManager, EasyNotepad and ParseWeb build with no change beyond the renamed names.
- The rule is written in the Lib README, so that a new shim follows it.
