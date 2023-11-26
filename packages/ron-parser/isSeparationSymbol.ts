// Space (ASCII 32)
const space = " ";
// Tab (ASCII 9)
const tab = "\t";
// Carriage Return (ASCII 13)
const carriageReturn = "\r";
// Form Feed (ASCII 12)
const formFeed = "\f";
// Vertical Tab (ASCII 11)
const verticalTab = "\v";

export function isSeparationSymbol(char: string) {
  switch (char) {
    case space:
      return true;
    case tab:
      return true;
    case carriageReturn:
      return true;
    case formFeed:
      return true;
    case verticalTab:
      return true;
    default:
      return false;
  }
}
