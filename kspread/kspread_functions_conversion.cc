// built-in conversion functions
#include "kspread_util.h"
#include "kspread_doc.h"
#include "kspread_table.h"

#include <koscript_parser.h>
#include <koscript_util.h>
#include <koscript_func.h>
#include <koscript_synext.h>

#include <stdlib.h>
#include <math.h>
#include <float.h>

#include <kdebug.h>

// Function: DECHEX
bool kspreadfunc_dec2hex( KSContext& context )
{
  QValueList<KSValue::Ptr>& args = context.value()->listValue();

  if ( !KSUtil::checkArgumentsCount( context, 1, "DECHEX", true ) ||!KSUtil::checkArgumentsCount( context, 1, "DEC2HEX", true ))
    return false;

  if ( !KSUtil::checkType( context, args[0], KSValue::IntType, true ) )
    return false;

  QString tmp;
  tmp=tmp.setNum( args[0]->intValue(),16);
  context.setValue( new KSValue( tmp ));

  return true;
}

// Function: DEC2OCT
bool kspreadfunc_dec2oct( KSContext& context )
{
  QValueList<KSValue::Ptr>& args = context.value()->listValue();

  if ( !KSUtil::checkArgumentsCount( context, 1, "DEC2OCT", true )  || !KSUtil::checkArgumentsCount( context, 1, "DECOCT", true ))
    return false;

  if ( !KSUtil::checkType( context, args[0], KSValue::IntType, true ) )
    return false;

  QString tmp;
  tmp=tmp.setNum( args[0]->intValue(),8);
  context.setValue( new KSValue( tmp ));

  return true;
}

// Function: DEC2BIN
bool kspreadfunc_dec2bin( KSContext& context )
{
  QValueList<KSValue::Ptr>& args = context.value()->listValue();

  if ( !KSUtil::checkArgumentsCount( context, 1, "DEC2BIN", true )  || !KSUtil::checkArgumentsCount( context, 1, "DECBIN", true ))
    return false;

  if ( !KSUtil::checkType( context, args[0], KSValue::IntType, true ) )
    return false;

  QString tmp;
  tmp=tmp.setNum( args[0]->intValue(),2);
  context.setValue( new KSValue( tmp ));

  return true;
}

// Function: BIN2DEC
bool kspreadfunc_bin2dec( KSContext& context )
{
  QValueList<KSValue::Ptr>& args = context.value()->listValue();

  if ( !KSUtil::checkArgumentsCount( context, 1, "BIN2DEC", true ) )
    return false;

  if ( !KSUtil::checkType( context, args[0], KSValue::StringType, true ) )
    return false;

  QString tmp=args[0]->stringValue();
  bool ok;
  long val=tmp.toLong(&ok,2);
  if(!ok)
        context.setValue( new KSValue( QString(i18n("Err") )));
  else
        context.setValue( new KSValue(val));

  return true;
}

// Function: BIN2OCT
bool kspreadfunc_bin2oct( KSContext& context )
{
  QValueList<KSValue::Ptr>& args = context.value()->listValue();

  if ( !KSUtil::checkArgumentsCount( context, 1, "BIN2OCT", true ) )
    return false;

  if ( !KSUtil::checkType( context, args[0], KSValue::StringType, true ) )
    return false;

  QString tmp=args[0]->stringValue();
  bool ok;
  long val=tmp.toLong(&ok,2);
  if(!ok)
        context.setValue( new KSValue( QString(i18n("Err") )));
  else
        {
        tmp=tmp.setNum(val,8);
        context.setValue( new KSValue(tmp));
        }

  return true;
}

// Function: BIN2HEX
bool kspreadfunc_bin2hex( KSContext& context )
{
  QValueList<KSValue::Ptr>& args = context.value()->listValue();

  if ( !KSUtil::checkArgumentsCount( context, 1, "BIN2HEX", true ) )
    return false;

  if ( !KSUtil::checkType( context, args[0], KSValue::StringType, true ) )
    return false;

  QString tmp=args[0]->stringValue();
  bool ok;
  long val=tmp.toLong(&ok,2);
  if(!ok)
        context.setValue( new KSValue( QString(i18n("Err") )));
  else
        {
        tmp=tmp.setNum(val,16);
        context.setValue( new KSValue(tmp));
        }

  return true;
}

// Function: OCT2DEC
bool kspreadfunc_oct2dec( KSContext& context )
{
  QValueList<KSValue::Ptr>& args = context.value()->listValue();

  if ( !KSUtil::checkArgumentsCount( context, 1, "OCT2DEC", true ) )
    return false;

  if ( !KSUtil::checkType( context, args[0], KSValue::StringType, true ) )
    return false;

  QString tmp=args[0]->stringValue();
  bool ok;
  long val=tmp.toLong(&ok,8);
  if(!ok)
        context.setValue( new KSValue( QString(i18n("Err") )));
  else
        context.setValue( new KSValue(val));

  return true;
}

// Function: OCT2BIN
bool kspreadfunc_oct2bin( KSContext& context )
{
  QValueList<KSValue::Ptr>& args = context.value()->listValue();

  if ( !KSUtil::checkArgumentsCount( context, 1, "OCT2BIN", true ) )
    return false;

  if ( !KSUtil::checkType( context, args[0], KSValue::StringType, true ) )
    return false;

  QString tmp=args[0]->stringValue();
  bool ok;
  long val=tmp.toLong(&ok,8);
  if(!ok)
        context.setValue( new KSValue( QString(i18n("Err") )));
  else
        {
        tmp=tmp.setNum(val,2);
        context.setValue( new KSValue(tmp));
        }

  return true;
}

// Function: OCT2HEX
bool kspreadfunc_oct2hex( KSContext& context )
{
  QValueList<KSValue::Ptr>& args = context.value()->listValue();

  if ( !KSUtil::checkArgumentsCount( context, 1, "OCT2HEX", true ) )
    return false;

  if ( !KSUtil::checkType( context, args[0], KSValue::StringType, true ) )
    return false;

  QString tmp=args[0]->stringValue();
  bool ok;
  long val=tmp.toLong(&ok,8);
  if(!ok)
        context.setValue( new KSValue( QString(i18n("Err") )));
  else
        {
        tmp=tmp.setNum(val,16);
        context.setValue( new KSValue(tmp));
        }

  return true;
}

// Function: HEX2DEC
bool kspreadfunc_hex2dec( KSContext& context )
{
  QValueList<KSValue::Ptr>& args = context.value()->listValue();

  if ( !KSUtil::checkArgumentsCount( context, 1, "HEX2DEC", true ) )
    return false;

  if ( !KSUtil::checkType( context, args[0], KSValue::StringType, true ) )
    return false;

  QString tmp=args[0]->stringValue();
  bool ok;
  long val=tmp.toLong(&ok,16);
  if(!ok)
        context.setValue( new KSValue( QString(i18n("Err") )));
  else
        context.setValue( new KSValue(val));

  return true;
}

// Function: HEX2BIN
bool kspreadfunc_hex2bin( KSContext& context )
{
  QValueList<KSValue::Ptr>& args = context.value()->listValue();

  if ( !KSUtil::checkArgumentsCount( context, 1, "HEX2BIN", true ) )
    return false;

  if ( !KSUtil::checkType( context, args[0], KSValue::StringType, true ) )
    return false;

  QString tmp=args[0]->stringValue();
  bool ok;
  long val=tmp.toLong(&ok,16);
  if(!ok)
        context.setValue( new KSValue( QString(i18n("Err") )));
  else
        {
        tmp=tmp.setNum(val,2);
        context.setValue( new KSValue(tmp));
        }

  return true;
}

// Function: HEX2OCT
bool kspreadfunc_hex2oct( KSContext& context )
{
  QValueList<KSValue::Ptr>& args = context.value()->listValue();

  if ( !KSUtil::checkArgumentsCount( context, 1, "HEX2OCT", true ) )
    return false;

  if ( !KSUtil::checkType( context, args[0], KSValue::StringType, true ) )
    return false;

  QString tmp=args[0]->stringValue();
  bool ok;
  long val=tmp.toLong(&ok,16);
  if(!ok)
        context.setValue( new KSValue( QString(i18n("Err") )));
  else
        {
        tmp=tmp.setNum(val,8);
        context.setValue( new KSValue(tmp));
        }

  return true;
}

// Function: POLR
bool kspreadfunc_polr( KSContext& context )
{
  QValueList<KSValue::Ptr>& args = context.value()->listValue();

  if ( !KSUtil::checkArgumentsCount( context,2, "POLR",true ) )
    return false;

  if ( !KSUtil::checkType( context, args[0], KSValue::DoubleType, true ) )
    return false;

  if ( !KSUtil::checkType( context, args[1], KSValue::DoubleType, true ) )
    return false;
  double result=sqrt(pow(args[0]->doubleValue(),2)+pow(args[1]->doubleValue(),2));
  context.setValue( new KSValue(result));

  return true;
}

// Function: POLA
bool kspreadfunc_pola( KSContext& context )
{
  QValueList<KSValue::Ptr>& args = context.value()->listValue();

  if ( !KSUtil::checkArgumentsCount( context,2, "POLA",true ) )
    return false;

  if ( !KSUtil::checkType( context, args[0], KSValue::DoubleType, true ) )
    return false;

  if ( !KSUtil::checkType( context, args[1], KSValue::DoubleType, true ) )
    return false;
  double result=acos(args[0]->doubleValue()/(sqrt(pow(args[0]->doubleValue(),2)+pow(args[1]->doubleValue(),2))));
  context.setValue( new KSValue(result));

  return true;
}

// Function: CARX
bool kspreadfunc_carx( KSContext& context )
{
  QValueList<KSValue::Ptr>& args = context.value()->listValue();

  if ( !KSUtil::checkArgumentsCount( context,2, "CARX",true ) )
    return false;

  if ( !KSUtil::checkType( context, args[0], KSValue::DoubleType, true ) )
    return false;

  if ( !KSUtil::checkType( context, args[1], KSValue::DoubleType, true ) )
    return false;
  double result=args[0]->doubleValue()*cos(args[1]->doubleValue());
  context.setValue( new KSValue(result));

  return true;
}

// Function: CARY
bool kspreadfunc_cary( KSContext& context )
{
  QValueList<KSValue::Ptr>& args = context.value()->listValue();

  if ( !KSUtil::checkArgumentsCount( context,2, "CARY",true ) )
    return false;

  if ( !KSUtil::checkType( context, args[0], KSValue::DoubleType, true ) )
    return false;

  if ( !KSUtil::checkType( context, args[1], KSValue::DoubleType, true ) )
    return false;
  double result=args[0]->doubleValue()*sin(args[1]->doubleValue());
  context.setValue( new KSValue(result));

  return true;
}

// Function: DECSEX
bool kspreadfunc_decsex( KSContext& context )
{
  QValueList<KSValue::Ptr>& args = context.value()->listValue();
  if ( !KSUtil::checkArgumentsCount( context,1, "DECSEX",true ) )
    return false;

  if ( !KSUtil::checkType( context, args[0], KSValue::DoubleType, true ) )
    return false;
  int inter;
  double val=args[0]->doubleValue();
  int hours,minutes,second;
  if(val>0)
    inter=1;
  else
    inter=-1;
  hours=inter*(int)(fabs(val));
  minutes=(int)(60*val-60*(int)(val));
  second=(int)(3600*val-3600*(int)(val)-60*(int)(60*val-60*(int)(val)));
  QTime _time(hours,minutes,second);
  context.setValue( new KSValue(_time));

  return true;
}

// Function: SEXDEC
bool kspreadfunc_sexdec( KSContext& context )
{
  QValueList<KSValue::Ptr>& args = context.value()->listValue();
  double result;
  if ( !KSUtil::checkArgumentsCount( context,3, "SEXDEC",true ) )
    {
      if ( !KSUtil::checkArgumentsCount( context,1, "SEXDEC",true ) )
	return false;
      if ( !KSUtil::checkType( context, args[0], KSValue::TimeType, true ) )
	return false;

      result=args[0]->timeValue().hour()+(double)args[0]->timeValue().minute()/60.0+(double)args[0]->timeValue().second()/3600.0;

      context.setValue( new KSValue(result));
    }
  else
    {
      if ( !KSUtil::checkType( context, args[0], KSValue::IntType, true ) )
	return false;
      if ( !KSUtil::checkType( context, args[1], KSValue::IntType, true ) )
	return false;
      if ( !KSUtil::checkType( context, args[2], KSValue::IntType, true ) )
	return false;
      result=args[0]->intValue()+(double)args[1]->intValue()/60.0+(double)args[2]->intValue()/3600.0;

      context.setValue( new KSValue(result));
    }

  return true;
}

// Function: ROMAN
bool kspreadfunc_roman( KSContext& context )
{
    const QCString RNUnits[] = {"", "I", "II", "III", "IV", "V", "VI", "VII", "VIII", "IX"};
    const QCString RNTens[] = {"", "X", "XX", "XXX", "XL", "L", "LX", "LXX", "LXXX", "XC"};
    const QCString RNHundreds[] = {"", "C", "CC", "CCC", "CD", "D", "DC", "DCC", "DCCC", "CM"};
    const QCString RNThousands[] = {"", "M", "MM", "MMM"};


    QValueList<KSValue::Ptr>& args = context.value()->listValue();
    if ( !KSUtil::checkArgumentsCount( context,1, "ROMAN",true ) )
        return false;
    int value;
    if ( !KSUtil::checkType( context, args[0], KSValue::IntType, true ) )
    {
        if ( !KSUtil::checkType( context, args[0], KSValue::DoubleType, true ) )
            return false;
        else
            value=(int)args[0]->doubleValue();
    }
    else
    	value=(int)args[0]->intValue();
    if(value<0)
    {
        context.setValue( new KSValue(i18n("Err")));
        return true;
    }
    if(value>3999)
    {
        context.setValue( new KSValue(i18n("Value too big")));
        return true;
    }
    QString result;

    result= QString::fromLatin1( RNThousands[ ( value / 1000 ) ] +
                                 RNHundreds[ ( value / 100 ) % 10 ] +
                                 RNTens[ ( value / 10 ) % 10 ] +
                                 RNUnits[ ( value ) % 10 ] );
    context.setValue( new KSValue(result));
    return true;
}

// Function: AsciiToChar
bool kspreadfunc_AsciiToChar( KSContext& context )
{
  QValueList<KSValue::Ptr>& args = context.value()->listValue();
  int val = -1;
  QString str;

  for (unsigned int i = 0; i < args.count(); i++)
  {
    if ( KSUtil::checkType( context, args[i], KSValue::IntType, false ) )
    {
      val = (int)args[i]->intValue();
      QChar c(val);
      str = str + c;
    }
    else return false;
  }

  context.setValue( new KSValue(str));
  return true;
}

// Function: CharToAscii
bool kspreadfunc_CharToAscii( KSContext& context )
{
  QValueList<KSValue::Ptr>& args = context.value()->listValue();

  if (args.count() == 1)
  {
    if ( KSUtil::checkType( context, args.first(), KSValue::StringType, false ) )
    {
      QString val = args[0]->stringValue();
      if (val.length() == 1)
      {
	QChar c = val[0];
	context.setValue( new KSValue(c.unicode() ));
	return true;
      }
    }
  }
  return false;
}

// Function: inttobool
bool kspreadfunc_inttobool( KSContext & context )
{
  QValueList<KSValue::Ptr>& args = context.value()->listValue();

  if (args.count() == 1)
  {
    if (KSUtil::checkType(context, args[0],
                          KSValue::IntType, true))
    {
      bool result = (args[0]->intValue() == 1 ? true : false);

      context.setValue( new KSValue(result) );

      return true;
    }
  }

  return false;
}

// Function: booltoint
bool kspreadfunc_booltoint( KSContext & context )
{
  QValueList<KSValue::Ptr>& args = context.value()->listValue();

  if (args.count() == 1)
  {
    if (KSUtil::checkType(context, args[0],
                          KSValue::BoolType, true))
    {
      int val = (args[0]->boolValue() ? 1 : 0);

      context.setValue( new KSValue(val));

      return true;
    }
  }

  return false;
}

// Function: BoolToString
bool kspreadfunc_BoolToString( KSContext& context )
{
  QValueList<KSValue::Ptr>& args = context.value()->listValue();

  if (args.count() == 1)
  {
    if ( KSUtil::checkType( context, args.first(), KSValue::BoolType, false ) )
    {
      QString val((args[0]->boolValue() ? "True" : "False"));

      context.setValue( new KSValue(val));

      return true;
    }
  }

  return false;
}

// Function: NumberToString
bool kspreadfunc_NumberToString( KSContext& context )
{
  QValueList<KSValue::Ptr>& args = context.value()->listValue();

  if (args.count() == 1)
  {
    if ( KSUtil::checkType( context, args.first(), KSValue::DoubleType, false ) )
    {
      QString val;
      val.setNum(args[0]->doubleValue(), 'g', 8);

      context.setValue( new KSValue(val));

      return true;
    }
  }

  return false;
}

// Function: Value
bool kspreadfunc_value( KSContext& context )
{
  QValueList<KSValue::Ptr>& args = context.value()->listValue();

  if ( !KSUtil::checkArgumentsCount( context, 1, "VALUE", true ) )
    return false;

  if ( !KSUtil::checkType( context, args[0], KSValue::StringType, true ) )
    return false;

  QString str = args[0]->stringValue();
  if( str.length() <= 0 )
     return false;

  // try to parse the string as number
  bool ok;
  double value = KGlobal::locale()->readNumber(str, &ok);
  if ( !ok )  value = str.toDouble(&ok);
  if( ok )
  {
     context.setValue( new KSValue( value ) );
     return true;
  }

  return false;
  // TODO parse as boolean/date/time
}
