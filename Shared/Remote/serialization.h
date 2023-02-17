#ifndef SERIALIZATION_H
#define SERIALIZATION_H

#include <QDataStream>
#include <QDebug>

class C_Player;
class C_Stack;
class C_LawBoard;
class C_ElectionTracker;
class C_GameTracker;

/*-----------------------------------------------------------------------------*/
/*                                C_Player                                     */
/*-----------------------------------------------------------------------------*/

QDebug operator<<(QDebug dbg, const C_Player &instance);

QDataStream & operator<<(QDataStream & stream, const C_Player &player);

QDataStream & operator>>(QDataStream & stream, C_Player &instance);

/*-----------------------------------------------------------------------------*/
/*                                  C_Stack                                     */
/*-----------------------------------------------------------------------------*/

QDebug operator<<(QDebug dbg, C_Stack &instance);

QDataStream & operator<<(QDataStream & stream, C_Stack &instance);

QDataStream & operator>>(QDataStream & stream, C_Stack &instance);

/*-----------------------------------------------------------------------------*/
/*                               C_LawBoard                                    */
/*-----------------------------------------------------------------------------*/

QDebug operator<<(QDebug dbg, const C_LawBoard &instance);

QDataStream & operator<<(QDataStream & stream, const C_LawBoard &instance);

QDataStream & operator>>(QDataStream & stream, C_LawBoard &instance);

/*-----------------------------------------------------------------------------*/
/*                           C_ElectionTracker                                 */
/*-----------------------------------------------------------------------------*/

QDebug operator<<(QDebug dbg, const C_ElectionTracker &instance);

QDataStream & operator<<(QDataStream & stream, const C_ElectionTracker &instance);

QDataStream & operator>>(QDataStream & stream, C_ElectionTracker &instance);

/*-----------------------------------------------------------------------------*/
/*                               C_GameTracker                                 */
/*-----------------------------------------------------------------------------*/

QDebug operator<<(QDebug dbg, const C_GameTracker &instance);

QDataStream & operator<<(QDataStream & stream, const C_GameTracker &instance);

QDataStream & operator>>(QDataStream & stream, C_GameTracker &instance);


#endif // SERIALIZATION_H
