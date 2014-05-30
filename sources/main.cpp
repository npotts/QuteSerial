/*
 *
 * This file is a part of QuteSerial (http://npotts.github.com/QuteSerial
 *
 * QuteSerial is free software; you can copy, modify, and distribute it
 * under the terms of the GNU Affero General Public License Version 2.
 *
 * QuteSerial is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public
 * License along with this program. If not, see
 * <http://www.gnu.org/licenses/>.
 */
#include <QApplication>
#include "quteserial.h"
//
int main(int argc, char ** argv) {
  QApplication app( argc, argv );
  Qute win;
  win.show();
  return app.exec();
}
