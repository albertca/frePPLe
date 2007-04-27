/***************************************************************************
  file : $URL$
  version : $LastChangedRevision$  $LastChangedBy$
  date : $LastChangedDate$
  email : jdetaeye@users.sourceforge.net
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 * Copyright (C) 2007 by Johan De Taeye                                    *
 *                                                                         *
 * This library is free software; you can redistribute it and/or modify it *
 * under the terms of the GNU Lesser General Public License as published   *
 * by the Free Software Foundation; either version 2.1 of the License, or  *
 * (at your option) any later version.                                     *
 *                                                                         *
 * This library is distributed in the hope that it will be useful,         *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of          *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser *
 * General Public License for more details.                                *
 *                                                                         *
 * You should have received a copy of the GNU Lesser General Public        *
 * License along with this library; if not, write to the Free Software     *
 * Foundation Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307, USA *
 *                                                                         *
 ***************************************************************************/

/** @file forecast.h
  * @brief Header file for the module forecast.
  *
  * @namespace module_forecast
  * @brief Module for representing forecast demands.
  *
  * Forecast demands behave different from other demands in the following
  * ways:
  *  - It models a hierarchical demand structure.
  *    The sub-demands are all managed automatically by the parent forecast
  *    demand, ie the sub-demands are transparent to the user.
  *    All sub-demands of a forecast demand have the same item.
  *    The due date is aligned on the buckets of the forecast calendar.
  *  - The forecast nets the actual demands. @todo
  *
  * The XML schema extension enabled by this module is (see mod_forecast.xsd):
  * <PRE>
  * <xsd:complexType name="DEMAND_FORECAST">
  *   <xsd:complexContent>
  *     <xsd:extension base="DEMAND">
  *       <xsd:choice minOccurs="0" maxOccurs="unbounded">
  *         <xsd:element name="CALENDAR" type="CALENDAR" />
  *         <xsd:element name="BUCKETS">
  *           <xsd:complexType>
  *             <xsd:choice minOccurs="0" maxOccurs="unbounded">
  *               <xsd:element name="BUCKET">
  *                 <xsd:complexType>
  *                   <xsd:all>
  *                     <xsd:element name="QUANTITY" type="positiveFloat"
  *                       minOccurs="0" />
  *                     <xsd:element name="DUE" type="xsd:dateTime"
  *                       minOccurs="0"/>
  *                   </xsd:all>
  *                   <xsd:attribute name="QUANTITY" type="positiveFloat" />
  *                   <xsd:attribute name="DUE" type="xsd:dateTime" />
  *                 </xsd:complexType>
  *               </xsd:element>
  *             </xsd:choice>
  *           </xsd:complexType>
  *         </xsd:element>
  *       </xsd:choice>
  *     </xsd:extension>
  *   </xsd:complexContent>
  * </xsd:complexType>
  * </PRE>
  */

#ifndef FORECAST_H
#define FORECAST_H

#include "frepple.h"
using namespace frepple;

namespace module_forecast
{


/** Initialization routine for the library. */
MODULE_EXPORT const char* initialize(const CommandLoadLibrary::ParameterList& z);


/** This class represents a bucketized demand signal.
  * The forecast object defines the item and priority of the demands.
  * A void calendar then defines the buckets.
  * The class basically works as an interface for a hierarchy of demands.
  */
class Forecast : public Demand
{
    TYPEDEF(Forecast);
  public:
    /** Constructor. */
    explicit Forecast(const string& nm) : Demand(nm), calptr(NULL) {}

    /** Updates the quantity of the forecast. This method is empty. */
    virtual void setQuantity(float f)
    {throw DataException("Can't set quantity of a forecast");}

    /** Update the forecast in a bucket, given any date in the bucket. */
    virtual void setQuantity(Date, float);

    void writeElement(XMLOutput*, const XMLtag&, mode=DEFAULT) const;
    void endElement(XMLInput& pIn, XMLElement& pElement);
    void beginElement(XMLInput& pIn, XMLElement& pElement);

    /** Update the item to be planned, for all buckets. */
    virtual void setItem(const Item*);

    /** Specify a bucket calendar for the forecast. Once forecasted
      * quantities have been entered for the forecast, the calendar
      * can't be updated any more. */
    virtual void setCalendar(const Calendar* c);

    /** Returns a reference to the calendar used for this forecast. */
    Calendar::pointer getCalendar() const {return calptr;}

    /** Updates the due date of the demand. Lower numbers indicate a
      * higher priority level. The method also updates the priority
      * in all buckets.
      */
    virtual void setPriority(int);

    /** Updates the operation being used to plan the demands. */
    virtual void setOperation(const Operation *);

    /** Updates the due date of the demand. */
    virtual void setDue(Date d)
    {throw DataException("Can't set due date of a forecast");}

    /** Update the policy of the demand in all buckets. */
    virtual void setPolicy(const string&);

    /** Update the policy of the demand in all buckets. */
    virtual void addPolicy(const string&);

    virtual const MetaClass& getType() const {return metadata;}
    static const MetaClass metadata;
    virtual size_t getSize() const
      {return sizeof(Forecast) + getName().size() + HasDescription::memsize();}

  private:
    /** A void calendar to define the time buckets. */
    const Calendar* calptr;

    /** Update the forecast in a bucket. */
    void setQuantity(const Calendar::Bucket&, float);
};


}   // End namespace

#endif
