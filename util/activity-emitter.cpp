/*
 * Copyright 2013-2017 Michael M. Magruder (https://github.com/mikemag)
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "activity-emitter.h"

#include <stdarg.h>

namespace MonkeyCAM {

/* static */ ActivityEmitter* ActivityEmitter::m_emitter = NULL;

/* static */ void ActivityEmitter::initialize(boost::optional<int> fd)
{
  m_emitter = new ActivityEmitter(fd);
}

ActivityEmitter::ActivityEmitter(boost::optional<int> fd)
    : m_jsonOutputFD(-1),
      m_jsonOutputFile(NULL)
{
  if (fd) {
    m_jsonOutputFD = *fd;
    m_jsonOutputFile = fdopen(m_jsonOutputFD, "w");
    printf("Outputting JSON to FD %d\n", m_jsonOutputFD);
    fprintf(m_jsonOutputFile, "["); // The reader expects an array of objects.
  }
}

void ActivityEmitter::write(const json& obj) {
  if (!m_jsonOutputFile) return;
  fprintf(m_jsonOutputFile, "%s,\n", obj.dump().c_str());
  fflush(m_jsonOutputFile);
}

void ActivityEmitter::write(const char* key, const char* fmt, ...) {
  if (!m_jsonOutputFile) return;
  va_list args;
  va_start(args, fmt);
  char buff[8192];
  vsnprintf(buff, sizeof(buff), fmt, args);
  va_end(args);
  fprintf(m_jsonOutputFile, R"({"%s": "%s"})", key, buff);
  fprintf(m_jsonOutputFile, ",\n");
  fflush(m_jsonOutputFile);
}

void ActivityEmitter::fatal(const char* fmt, ...) {
  va_list args;
  va_start(args, fmt);
  char buff[8192];
  vsnprintf(buff, sizeof(buff), fmt, args);
  va_end(args);
  printf("%s\n", buff);
  if (!m_jsonOutputFile) return;
  fprintf(m_jsonOutputFile, R"({"fatalError": "%s"})", buff);
  fprintf(m_jsonOutputFile, ",\n");
  fflush(m_jsonOutputFile);
}

} // namespace MonkeyCAM
