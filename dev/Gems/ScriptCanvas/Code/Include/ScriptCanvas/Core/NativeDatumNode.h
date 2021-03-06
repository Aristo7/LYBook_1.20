/*
* All or portions of this file Copyright (c) Amazon.com, Inc. or its affiliates or
* its licensors.
*
* For complete copyright and license terms please see the LICENSE at the root of this
* distribution (the "License"). All use of this software is governed by the License,
* or, if provided, by the license below or the license accompanying this file. Do not
* remove or modify any license notices. This file is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*
*/

#pragma once

#include <Data/Data.h>
#include <ScriptCanvas/Core/Node.h>
#include <ScriptCanvas/Core/PureData.h>
#include <AzCore/std/typetraits/function_traits.h>

namespace ScriptCanvas
{
    namespace Nodes
    {
        template<typename t_Node, typename t_Datum>
        class NativeDatumNode
            : public PureData
        {
        public:
            using t_ThisType = NativeDatumNode<t_Node, t_Datum>;

            AZ_RTTI(((NativeDatumNode<t_Node, t_Datum>), "{B7D8D8D6-B2F1-481A-A712-B07D1C19555F}", t_Node, t_Datum), PureData, AZ::Component);
            AZ_COMPONENT_INTRUSIVE_DESCRIPTOR_TYPE(NativeDatumNode);
            AZ_COMPONENT_BASE(NativeDatumNode, PureData);

            static void Reflect(AZ::ReflectContext* reflection) 
            {
                if (AZ::SerializeContext* serializeContext = azrtti_cast<AZ::SerializeContext*>(reflection))
                {
                    serializeContext->Class<t_ThisType, PureData>()
                        ->Version(0)
                        ;

                    if (AZ::EditContext* editContext = serializeContext->GetEditContext())
                    {
                        editContext->Class<t_ThisType>("NativeDatumNode", "")
                            ->ClassElement(AZ::Edit::ClassElements::EditorData, "")
                            ;
                    }
                }
            }

            ~NativeDatumNode() override = default;

        protected:
            virtual void ConfigureSetters()
            {
                Data::SetterContainer setterWrappers = Data::ExplodeToSetters(Data::FromAZType(Data::Traits<t_Datum>::GetAZType()));
                for (const auto& setterWrapperPair : setterWrappers)
                {
                    SlotId setterSlotId;
                    const Data::SetterWrapper& setterWrapper = setterWrapperPair.second;
                    const AZStd::string argName = AZStd::string::format("%s: %s", Data::GetName(setterWrapper.m_propertyType).data(), setterWrapper.m_propertyName.data());
                    AZStd::string_view argumentTooltip;
                    // Add the slot if it doesn't exist
                    if (!SlotExists(argName, SlotType::DataIn, setterSlotId))
                    {
                        setterSlotId = AddInputTypeSlot(argName, argumentTooltip, setterWrapper.m_propertyType, InputTypeContract::DatumType);
                    }

                    if (setterSlotId.IsValid())
                    {
                        m_propertyAccount.m_getterSetterIdPairs[setterWrapperPair.first].second = setterSlotId;
                        m_propertyAccount.m_settersByInputSlot.emplace(setterSlotId, setterWrapperPair.second);
                    }
                }
            }

            virtual void ConfigureGetters()
            {
                Data::GetterContainer getterWrappers = Data::ExplodeToGetters(Data::FromAZType(Data::Traits<t_Datum>::GetAZType()));
                for (const auto& getterWrapperPair : getterWrappers)
                {
                    SlotId getterSlotId;

                    const Data::GetterWrapper& getterWrapper = getterWrapperPair.second;
                    const AZStd::string resultSlotName(AZStd::string::format("%s: %s", getterWrapper.m_propertyName.data(), Data::GetName(getterWrapper.m_propertyType).data()));
                    // Add the slot if it doesn't exist
                    if (!SlotExists(resultSlotName, SlotType::DataOut, getterSlotId))
                    {
                        getterSlotId = AddOutputTypeSlot(resultSlotName, {}, getterWrapper.m_propertyType, OutputStorage::Optional);
                    }

                    if (getterSlotId.IsValid())
                    {
                        m_propertyAccount.m_getterSetterIdPairs[getterWrapperPair.first].first = getterSlotId;
                        m_propertyAccount.m_gettersByInputSlot.emplace(getterSlotId, getterWrapperPair.second);
                    }
                }
            }

            virtual void ConfigureProperties()
            {
                if (IsConfigured())
                {
                    return;
                }

                ConfigureGetters();
                ConfigureSetters();
                m_configured = true;
            }

            void OnInit() override
            {
                AddInputAndOutputTypeSlot(Data::FromAZType<t_Datum>());
                ConfigureProperties();
            }
        };
    }
}