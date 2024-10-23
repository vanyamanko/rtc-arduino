bool isLeapYear(int year) {
  return ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0));
}

bool isValidDate(int day, int month, int year) {
  if (year < 1) return false;

  if (month < 1 || month > 12) return false;

  int daysInMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

  if (isLeapYear(year) && month == 2) {
    daysInMonth[1] = 29;
  }

  if (day < 1 || day > daysInMonth[month - 1]) return false;

  return true;
}

bool checkValidDateString(String dateStr) {
  if (dateStr.length() != 10 || dateStr[2] != '-' || dateStr[5] != '-') {
    return false;
  }

  int day = dateStr.substring(0, 2).toInt();
  int month = dateStr.substring(3, 5).toInt();
  int year = dateStr.substring(6, 10).toInt();

  return isValidDate(day, month, year);
}



bool isValidTime(int hours, int minutes, int seconds) {
  if (hours < 0 || hours > 23) return false;

  if (minutes < 0 || minutes > 59) return false;

  if (seconds < 0 || seconds > 59) return false;

  return true;
}

bool checkValidTimeString(String timeStr) {
  if (timeStr.length() != 8 || timeStr[2] != ':' || timeStr[5] != ':') {
    return false;
  }

  int hours = timeStr.substring(0, 2).toInt();
  int minutes = timeStr.substring(3, 5).toInt();
  int seconds = timeStr.substring(6, 8).toInt();

  return isValidTime(hours, minutes, seconds);
}
